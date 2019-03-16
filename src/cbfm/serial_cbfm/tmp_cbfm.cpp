#include "tmp_cbfm.h"

void tperformCBFM(std::map<std::string, std::string> &const_map,
				 std::map<int, Label> &label_map,
                 std::vector<Triangle> &triangles,
                 std::vector<Edge> &edges,
                 std::vector<Node<double>> &nodes,
                 std::complex<double> *ilhs,
                 bool fpga)
{
	std::ofstream file;
	file.open("cbfm.txt");

	// Define some constants
	int num_domains = label_map.size(); 				// number of domains
	int domain_size = label_map[0].edge_indices.size();		// number of edges in domain
   	int z_lda = std::max(1, domain_size);				// lda for LAPACK
   	int j_lda = std::max(1, domain_size);				// lda for LAPACK
	int info = 0;							// needed for LAPACK
	int one = 1;							// needed for LAPACK
	char tran = 'T';						// needed for LAPACK
	char norm = 'N';						// needed for LAPACK
	int index;							// indexing variable
	std::complex<double> minus_one = std::complex<double>(-1.0,0.0);// needed for LAPACK
	std::complex<double> c_zero = std::complex<double>(0.0, 0.0);	// needed for LAPACK
	std::complex<double> c_one = std::complex<double>(1.0, 0.0);	// needed for LAPACK
	

	// Create struct to store all z matrices and v vectors
	CBFMZMatrices v_mom_z;
	CBFMVectors v_mom_v;

	// Lets resize all the struct elements to fit the problem
	// NB: Be very aware that for v_mom_z.z_couple and v_mom_v.j_sec contain NULL 
	//     pointers at the self terms i.e. v_mom_z.z_couple[i][j] when i==j.
	//     This is done to allow multiple steps to go into the same loop for
	//     speed. This is the most efficient indexing method thought of.
	//
	// TODO: Find a better indexing method
	resizeCBFMZMatricesForEqualDomains(v_mom_z, num_domains, domain_size);	
	resizeCBFMVectorsForEqualDomains(v_mom_v, num_domains, domain_size);

	// Lets fill v_mom_z.z_self
	serialFillZmn(v_mom_z.z_self,
		      edges,
		      triangles,
		      nodes,
		      const_map,
		      label_map[0],
		      label_map[0],
		      false);
	
	// Now copy z_self to z_self_inv because LAPACK overwites the original matrix
	memcpy(v_mom_z.z_self_inv,
   		   v_mom_z.z_self,
   		   domain_size * domain_size * sizeof(std::complex<double>));

	// Now get the LU-decomp of z_self_inv
	zgetrf_(&domain_size, &domain_size, v_mom_z.z_self_inv, &z_lda, v_mom_z.z_self_piv, &info); 

	// Lets now fill v_self vectors
	for(int i = 0; i < num_domains; i++)
	{
		// fill v_self
		serialFillVrhs(const_map,
			       triangles,
			       edges,
			       v_mom_v.v_self[i],
			       label_map[i]);

		// Copy v_self to j_prim because of LAPACK overwrites
		memcpy(v_mom_v.j_prim[i], v_mom_v.v_self[i], domain_size * sizeof(std::complex<double>));
	}

	// Lets now do four things
	// 1. Calculate primary CBF's
	// 2. Fill in coupling z matrices -> (v_mom_z.z_couple)
	// 3. Calculate secondary CBF's
	// 4. Concatenate primary and secondary CBF's
	// All together in a double for loop for speed

	std::vector<int> copy_index(num_domains, 1);

	for(int i = 0; i < num_domains; i++)
	{
	  // Caclulate primary CBF's
    	zgetrs_(&tran, &domain_size, &one, v_mom_z.z_self_inv, &domain_size,
    	        v_mom_z.z_self_piv, v_mom_v.j_prim[i], &domain_size, &info);

    	// Copy primary CBF's co concatenated matrix -> (v_mom_v.j_cbfm)
    	std::copy(v_mom_v.j_prim[i], v_mom_v.j_prim[i] + domain_size, v_mom_v.j_cbfm[i]);

		for(int j = 0; j < num_domains; j++)
		{
			if(j != i)
			{
				// Fill in coupling z matrices
				serialFillZmn(v_mom_z.z_couple[j][i],
					      edges,
					      triangles,
					      nodes,
					      const_map,
					      label_map[j],
					      label_map[i],
					      true);   
				
				// Multiply coupling z with primary CBF's and with -1
				// Send in v_mom_v.j_sec for LAPACK to write to
				zgemv_(&tran, &domain_size, &domain_size, &minus_one, v_mom_z.z_couple[j][i], &z_lda,
				       v_mom_v.j_prim[i], &one, &c_zero, v_mom_v.j_sec[j][i], &one);	

				// Calculate secondary CBF's
				zgetrs_(&tran, &domain_size, &one, v_mom_z.z_self_inv, &domain_size,
					v_mom_z.z_self_piv, v_mom_v.j_sec[j][i], &domain_size, &info);

				// Copy secondary CBF's to concatenated matrix -> (v_mom_v.j_cbfm)
				std::copy(v_mom_v.j_sec[j][i], v_mom_v.j_sec[j][i] + domain_size,
					  v_mom_v.j_cbfm[j] + (copy_index[j] * domain_size));

				copy_index[j]++;
			}
		}
	}

    //---------------------------------------------------------
    
    
    //---------------------------------------------------------

	// Lets now calculate the reduced matrices and vectors
	// Also lets add in a hook for FPGA shenanigans
	if(fpga)
	{
        // FPGA stuff
        //
		// NB: Make sure that the matrices going in and out are of the correct major.
		// 	   Fortran, and by extension, LAPACK, use column_major indexing when compared to the row_major indexing
		//     that is used normally.
		//
		// Send concatenated CBF's 		-> amount: num_domains					   	-> From v_mom_v.j_cbfm             -> column_major
		// Send self Zmn matrix 		-> amount: 1							   	-> From z_mom_z.z_self 			   -> row_major
		// Send coupling Zmn matrices	-> amount: num_domains * (num_domains - 1) 	-> From z_mom_z.z_couple           -> row_major
		// 
		// Get reduced Zcbfm			-> amount: 1 								-> Place into z_mom_z.z_red_concat -> column_major
		// Get reduced Vcbfm			-> amount: 1 								-> Place into v_mom_v.v_red_concat -> any
	}
	else
	{
		// Create temporary matrix for LAPACK to store intermediary matrix
		std::complex<double> *c_temp = new std::complex<double>[num_domains * domain_size];	
		int disp_index;

		// Lets now loop and solve the reduced z matrices and v_vectors
		for(int i = 0; i < num_domains; i++)
		{
			index = 0;
			disp_index = i * num_domains;

			// Calculate the reduced v vector
			zgemv_(&tran, &domain_size, &num_domains, &c_one, v_mom_v.j_cbfm[i], &domain_size,
			       v_mom_v.v_self[i], &one, &c_zero, v_mom_v.v_red[i], &one);

			// Copy reduced v vector to concatenated vector
			std::copy(v_mom_v.v_red[i], v_mom_v.v_red[i] + num_domains, v_mom_v.v_red_concat + (i * num_domains));

			for(int j = 0; j < num_domains; j++)
			{
				if(j == i)
				{
					// Calculate Zrwg_self * Jcbfm 
				    zgemm_(&tran, &norm, &domain_size, &num_domains, &domain_size, &c_one, v_mom_z.z_self,
					   &z_lda, v_mom_v.j_cbfm[i], &j_lda, &c_zero, c_temp, &z_lda);	

				    // Calculate Zcbfm^-1 * [Zrwg_self * Jcbfm]
				    zgemm_(&tran, &norm, &num_domains, &num_domains, &domain_size, &c_one, v_mom_v.j_cbfm[i],
					   &domain_size, c_temp, &domain_size, &c_zero, v_mom_z.z_red[i][j], &num_domains);
				}
				else
				{
					// Calculate Zrwg_couple * Jcbfm 
					zgemm_(&tran, &norm, &domain_size, &num_domains, &domain_size, &c_one, v_mom_z.z_couple[i][j],
					       &z_lda, v_mom_v.j_cbfm[j], &j_lda, &c_zero, c_temp, &z_lda);	

				    // Calculate Zcbfm^-1 * [Zrwg_couple * Jcbfm]
					zgemm_(&tran, &norm, &num_domains, &num_domains, &domain_size, &c_one, v_mom_v.j_cbfm[i],
					       &domain_size, c_temp, &domain_size, &c_zero, v_mom_z.z_red[i][j], &num_domains);
				}

				// Now move reduced Zcbfm to concatenated matrix
				// NB: Do it in a column_major order because the reduced matrices, v_mom_z.z_red,
				//     are received from LAPACK in that manner
				for(int k = 0; k < num_domains; k++)
				{
					if(k == 0)
					{
						std::copy(v_mom_z.z_red[i][j],
							  v_mom_z.z_red[i][j] + num_domains,
							  v_mom_z.z_red_concat + disp_index);
 						disp_index += num_domains * num_domains; 	
					}
					else
					{
						std::copy(v_mom_z.z_red[i][j] + (k * num_domains),
							  v_mom_z.z_red[i][j] + ((k + 1) * num_domains),
							  v_mom_z.z_red_concat + disp_index);
						disp_index += num_domains * num_domains;
					}
				}	
			}
		}
	}

	//-- Solve Irwg --//
    file << "---------------------------------------Z_REDC------------------------------------------" << std::endl;
	for(int i = 0; i < (num_domains * num_domains); i++)
	{
		for(int j = 0; j < (num_domains * num_domains); j++)
		{
			file << v_mom_z.z_red_concat[j + i * (num_domains * num_domains)];
		}
		file << std::endl;
	}
	file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;
	int num_red_rows = num_domains * num_domains;
	int z_red_piv[num_red_rows];

	// Calculate Icbfm
	zgetrf_(&num_red_rows, &num_red_rows, v_mom_z.z_red_concat, &num_red_rows, z_red_piv, &info); 
	zgetrs_(&norm, &num_red_rows, &one, v_mom_z.z_red_concat, &num_red_rows,
		z_red_piv, v_mom_v.v_red_concat, &num_red_rows, &info);

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "FROM OLD" << std::endl;
	for(int i = 0; i < 9; i++)
	{
		std::cout << v_mom_v.v_red_concat[i] << std::endl;
	}

	index = 0;
	int rwg_pos = 0;
	
	// Map CBF's scaled by Icbfm to Irwg  
  	for(int i = 0; i < num_domains; i++)
	  {
	    zaxpy_(&domain_size, &v_mom_v.v_red_concat[index], v_mom_v.j_prim[i], &one, ilhs + rwg_pos, &one);
	    index++;
	    
	    for(int j = 0; j < num_domains; j++)
	      {	
		if(j != i)
		  {
		    zaxpy_(&domain_size, &v_mom_v.v_red_concat[index], v_mom_v.j_sec[i][j], &one, ilhs + rwg_pos, &one);
		    index++;
		  }
	      }
	    rwg_pos += domain_size;
	  } 

  	// DEBUG --------------------------------------------------------------------------------------------------------------------

	// file << "---------------------------------------Z_SELF------------------------------------------" << std::endl;
	// for(int i = 0; i < domain_size; i++)
	// {
	// 	for(int j = 0; j < domain_size; j++)
	// 	{
	// 		file << v_mom_z.z_self[j + i * domain_size];
	// 	}
	// 	file << std::endl;
	// }
	// file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;

	// file << "---------------------------------------Z_SINV------------------------------------------" << std::endl;
	// for(int i = 0; i < domain_size; i++)
	// {
	// 	for(int j = 0; j < domain_size; j++)
	// 	{
	// 		file << v_mom_z.z_self_inv[j + i * domain_size];
	// 	}
	// 	file << std::endl;
	// }
	// file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;

	// for(int m = 0; m < num_domains; m++){
 //    for(int n = 0; n < (num_domains); n++){	
 //    if(m!=n){
 //    file << "--------------------------------------Z_COUP"<<m<<n<<"-----------------------------------------" << std::endl;
 //    for(int i = 0; i < domain_size; i++)
 //    {
 //        for(int j = 0; j < domain_size; j++)
 //        {
 //            file << v_mom_z.z_couple[m][n][j + i * domain_size];
 //        }
 //        file << std::endl;
 //    }
 //    file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;
 //    }}}

 //    for(int m = 0; m < num_domains; m++){
 //    file << "--------------------------------------V_SELF"<<m<<"------------------------------------------" << std::endl;
 //    for(int i = 0; i < domain_size; i++)
 //    {
 //    	file << v_mom_v.v_self[m][i] << std::endl;
 //    }
 //    file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;
 //    }

	for(int m = 0; m < num_domains; m++){
	file << "--------------------------------------J_PRIM"<<m<<"------------------------------------------" << std::endl;
    for(int i = 0; i < domain_size; i++)
    {
    	file << v_mom_v.j_prim[m][i] << std::endl;
    }
    file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;
    }

	for(int m = 0; m < num_domains; m++){
    for(int n = 0; n < (num_domains); n++){
    if(m!=n){	
    file << "--------------------------------------J_SECC"<<m<<n<<"-----------------------------------------" << std::endl;
    for(int i = 0; i < domain_size; i++)
    {      
        file << v_mom_v.j_sec[m][n][i] << std::endl;
    }
    file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;
    }}}

 //    for(int m = 0; m < num_domains; m++){
	// file << "--------------------------------------J_CBFM"<<m<<"------------------------------------------" << std::endl;
 //    for(int i = 0; i < num_domains; i++)
 //    {
 //    	for(int j = 0; j < domain_size; j++)
 //    	{
 //    		file << v_mom_v.j_cbfm[m][j + i * domain_size];
 //    	}
 //    	file << std::endl;
 //    }
 //    file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;
 //    }

    for(int m = 0; m < num_domains; m++){
    for(int n = 0; n < num_domains; n++){	
    file << "--------------------------------------Z_REDD"<<m<<n<<"-----------------------------------------" << std::endl;
    for(int i = 0; i < num_domains; i++)
    {
        for(int j = 0; j < num_domains; j++)
        {
            file << v_mom_z.z_red[m][n][j + i * num_domains];
        }
        file << std::endl;
    }
    file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;
    }}

 //    for(int m = 0; m < num_domains; m++){
 //    file << "--------------------------------------V_REDD"<<m<<"------------------------------------------" << std::endl;
 //    for(int i = 0; i < num_domains; i++)
 //    {
 //            file << v_mom_v.v_red[m][i] << std::endl;
 //    }
 //    file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;
 //    }

    file << "---------------------------------------Z_REDC------------------------------------------" << std::endl;
	for(int i = 0; i < (num_domains * num_domains); i++)
	{
		for(int j = 0; j < (num_domains * num_domains); j++)
		{
			file << v_mom_z.z_red_concat[j + i * (num_domains * num_domains)];
		}
		file << std::endl;
	}
	file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;

	// file << "---------------------------------------V_REDC------------------------------------------" << std::endl;
	// for(int i = 0; i < (num_domains * num_domains); i++)
	// {
				
	// 	file << v_mom_v.v_red_concat[i];
	// 	file << std::endl;
	// }
	// file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;



	file.close();

}
