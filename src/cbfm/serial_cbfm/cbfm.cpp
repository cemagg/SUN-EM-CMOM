#include "cbfm.h"

void performCBFM(std::map<std::string, std::string> &const_map,
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
	int num_domains = label_map.size(); 							// number of domains
	int domain_size = label_map[0].edge_indices.size();				// number of edges in domain
   	int z_lda = std::max(1, domain_size);							// lda for LAPACK
   	int j_lda = std::max(1, domain_size);							// lda for LAPACK
   	int j_222 = std::max(1, num_domains);							// lda for LAPACK
    int info = 0;													// needed for LAPACK
    int one = 1;													// needed for LAPACK
    char tran = 'T';												// needed for LAPACK
    char norm = 'N';												// needed for LAPACK
    int index;														// indexing variable
	std::complex<double> minus_one = std::complex<double>(-1.0,0.0);// needed for LAPACK
	std::complex<double> c_zero = std::complex<double>(0.0, 0.0);	// needed for LAPACK
	std::complex<double> c_one = std::complex<double>(1.0, 0.0);	// needed for LAPACK


	// Create struct to store all z matrices and v vectors
	CBFMZMatrices v_mom_z; 
    CBFMVectors v_mom_v;


	// Lets resize all the struct elements to fit the problem
	resizeCBFMZMatricesForEqualDomains(v_mom_z, num_domains, label_map[0].edge_indices.size());	
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

	for(int i = 0; i < num_domains; i++)
	{
		index = 0;

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
				serialFillZmn(v_mom_z.z_couple[j][index],
					  		  edges,
				  			  triangles,
				  			  nodes,
                  		 	  const_map,
                  		  	  label_map[j],
                  		  	  label_map[i],
                  		  	  true);   
				
				// Multiply coupling z with primary CBF's and with -1
				// Send in v_mom_v.j_sec for LAPACK to write to
				zgemv_(&tran, &domain_size, &domain_size, &minus_one, v_mom_z.z_couple[j][index], &z_lda,
                	v_mom_v.j_prim[i], &one, &c_zero, v_mom_v.j_sec[j][index], &one);	

				// Calculate secondary CBF's
				zgetrs_(&tran, &domain_size, &one, v_mom_z.z_self_inv, &domain_size,
    	                v_mom_z.z_self_piv, v_mom_v.j_sec[j][index], &domain_size, &info);

				// Copy secondary CBF's to concatenated matrix -> (v_mom_v.j_cbfm)
				std::copy(v_mom_v.j_sec[j][index], v_mom_v.j_sec[j][index] + domain_size,
						  v_mom_v.j_cbfm[j] + ((index+1) * domain_size));

				index++;
			}
		}
	}


	// Lets now calculate the reduced matrices and vectors
	// Also lets add in a hook for FPGA shenanigans
	if(fpga)
	{
		// FPGA stuff
	}
	else
	{
		// Create temporary matrix for LAPACK to store intermediary matrix
		std::complex<double> *c_temp = new std::complex<double>[num_domains * domain_size];	
		bool fl = true;
		std::complex<double> *CME = new std::complex<double>[num_domains];
		int index_one = 0;
		int index_two = 0;

		// Lets now loop and solve the reduced z matrices and v_vectors
		for(int i = 0; i < num_domains; i++)
		{
			// Calculate the reduced v vector
			zgemv_(&tran, &domain_size, &num_domains, &c_one, v_mom_v.j_cbfm[i], &domain_size,
				   v_mom_v.v_self[i], &one, &c_zero, v_mom_v.v_red[i], &one);

			// Copy reduced v vector to concatenated vector
			// std::copy(v_mom_v.v_red[i], v_mom_v.v_red[i] + num_domains, v_mom_v.v_red_concat);

			for(int j = 0; j < num_domains; j++)
			{
				if(j == 0)
				{
				    zgemm_(&tran, &norm, &domain_size, &num_domains, &domain_size, &c_one, v_mom_z.z_self,
    	        		   &z_lda, v_mom_v.j_cbfm[i], &j_lda, &c_zero, c_temp, &z_lda);	

				    zgemm_(&tran, &norm, &num_domains, &num_domains, &domain_size, &c_one, v_mom_v.j_cbfm[i],
    	        		   &domain_size, c_temp, &domain_size, &c_zero, v_mom_z.z_red[i][j], &num_domains);
				}
				else
				{
					zgemm_(&tran, &norm, &domain_size, &num_domains, &domain_size, &c_one, v_mom_z.z_couple[i][(j-1)],
    	        		   &z_lda, v_mom_v.j_cbfm[j], &j_lda, &c_zero, c_temp, &z_lda);	

					zgemm_(&tran, &norm, &num_domains, &num_domains, &domain_size, &c_one, v_mom_v.j_cbfm[i],
    	        		   &domain_size, c_temp, &domain_size, &c_zero, v_mom_z.z_red[i][j], &num_domains);
				}

				for(int k = 0; k < num_domains; k++)
				{
					if(k == 0)
					{
						index_one =  (i * num_domains * num_domains * num_domains) + 
									 (j * num_domains);
 						index_two = index_one + (num_domains * num_domains);	

						std::copy(v_mom_z.z_red[i][j],
								  v_mom_z.z_red[i][j] + num_domains,
								  v_mom_z.z_red_concat + index_one);
					}
					else
					{
						std::copy(v_mom_z.z_red[i][j] + (k * num_domains),
								  v_mom_z.z_red[i][j] + ((k + 1) * num_domains),
								  v_mom_z.z_red_concat + index_two);
						index_two += (num_domains * num_domains); 
					}
				}	
			}
		}
	}

	for(int i = 0; i < 4; i++)
	{
		std::cout << v_mom_z.z_red[0][0][i] << std::endl;
	}
	//-- Solve Irwg --//
















	file << "---------------------------------------Z_SELF------------------------------------------" << std::endl;
	for(int i = 0; i < domain_size; i++)
	{
		for(int j = 0; j < domain_size; j++)
		{
			file << v_mom_z.z_self[j + i * domain_size];
		}
		file << std::endl;
	}
	file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;

	file << "---------------------------------------Z_SINV------------------------------------------" << std::endl;
	for(int i = 0; i < domain_size; i++)
	{
		for(int j = 0; j < domain_size; j++)
		{
			file << v_mom_z.z_self_inv[j + i * domain_size];
		}
		file << std::endl;
	}
	file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;

	for(int m = 0; m < num_domains; m++){
    for(int n = 0; n < (num_domains - 1); n++){	
    file << "--------------------------------------Z_COUP"<<m<<n<<"-----------------------------------------" << std::endl;
    for(int i = 0; i < domain_size; i++)
    {
        for(int j = 0; j < domain_size; j++)
        {
            file << v_mom_z.z_couple[m][n][j + i * domain_size];
        }
        file << std::endl;
    }
    file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;
    }}

    for(int m = 0; m < num_domains; m++){
    file << "--------------------------------------V_SELF"<<m<<"------------------------------------------" << std::endl;
    for(int i = 0; i < domain_size; i++)
    {
    	file << v_mom_v.v_self[m][i] << std::endl;
    }
    file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;
    }

	for(int m = 0; m < num_domains; m++){
	file << "--------------------------------------J_PRIM"<<m<<"------------------------------------------" << std::endl;
    for(int i = 0; i < domain_size; i++)
    {
    	file << v_mom_v.j_prim[m][i] << std::endl;
    }
    file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;
    }

	for(int m = 0; m < num_domains; m++){
    for(int n = 0; n < (num_domains - 1); n++){	
    file << "--------------------------------------J_SECC"<<m<<n<<"-----------------------------------------" << std::endl;
    for(int i = 0; i < domain_size; i++)
    {      
        file << v_mom_v.j_sec[m][n][i] << std::endl;
    }
    file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;
    }}

    for(int m = 0; m < num_domains; m++){
	file << "--------------------------------------J_CBFM"<<m<<"------------------------------------------" << std::endl;
    for(int i = 0; i < num_domains; i++)
    {
    	for(int j = 0; j < domain_size; j++)
    	{
    		file << v_mom_v.j_cbfm[m][j + i * domain_size];
    	}
    	file << std::endl;
    }
    file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;
    }

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

    for(int m = 0; m < num_domains; m++){
    file << "--------------------------------------V_REDD"<<m<<"------------------------------------------" << std::endl;
    for(int i = 0; i < num_domains; i++)
    {
            file << v_mom_v.v_red[m][i] << std::endl;
    }
    file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;
    }

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
	file.close();

}