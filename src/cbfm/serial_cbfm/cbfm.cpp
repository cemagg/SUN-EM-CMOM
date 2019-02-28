#include "cbfm.h"

void performCBFM(std::map<std::string, std::string> &const_map,
				 std::map<int, Label> &label_map,
                   std::vector<Triangle> &triangles,
                   std::vector<Edge> &edges,
                   std::vector<Node<double>> &nodes,
                   std::complex<double> *ilhs)
{
	std::ofstream file;
	file.open("cbfm.txt");

	// Define some constants
	int num_domains = label_map.size(); // number of domains
	int domain_size = label_map[0].edge_indices.size();

	//-- Create necessary Zmn Matrices --//
	
	// Create struct to store all z matrices and their inverses
	CBFMZMatrices v_mom_z; // Struct storing the vanilla mom Zmn matrices

	// Lets resize all the struct elements to fit the problem
	resizeCBFMZMatricesForEqualDomains(v_mom_z, num_domains, label_map[0].edge_indices.size());	

	// Lets fill the Z matrices

	// Lets first fill v_mom_z.z_self
	serialFillZmn(v_mom_z.z_self,
				  edges,
				  triangles,
				  nodes,
                  const_map,
                  label_map[0],
                  label_map[0],
                  false);
	memcpy(v_mom_z.z_self_inv,
   		   v_mom_z.z_self,
   		   domain_size * domain_size * sizeof(std::complex<double>));

   	int z_lda = std::max(1, domain_size);
    int info = 0;
    int one = 1;
    char tran = 'T';

    zgetrf_(&domain_size, &domain_size, v_mom_z.z_self_inv, &z_lda, v_mom_z.z_self_piv, &info); 

	// Lets now fill in v_mom_z.z_couple
	int index;
    for(int i = 0; i < num_domains; i++)
	{
        index = 0;

		for(int j = 0; j < num_domains; j++)
		{
			if(j != i)
			{
				serialFillZmn(v_mom_z.z_couple[i][index],
					  		  edges,
				  			  triangles,
				  			  nodes,
                  		 	  const_map,
                  		  	  label_map[i],
                  		  	  label_map[j],
                  		  	  true);            

				memcpy(v_mom_z.z_couple_inv[i][index],
					   v_mom_z.z_couple[i][index],
					   domain_size * domain_size * sizeof(std::complex<double>));

				zgetrf_(&domain_size, &domain_size, v_mom_z.z_couple_inv[i][index],
					    &z_lda, v_mom_z.z_couple_piv[i][index], &info); 

                index++; 
			}
		}	
	}

	//-- Create necessary Vm vectors --//
	CBFMVectors v_mom_v;

	// Lets resize some of the cbfm vectors
	resizeCBFMVectorsForEqualDomains(v_mom_v, num_domains, domain_size);

	// Lets now fill v_self vectors
	for(int i = 0; i < num_domains; i++)
	{
		serialFillVrhs(const_map,
					   triangles,
					   edges,
					   v_mom_v.v_self[i],
					   label_map[i]);
		mempcpy(v_mom_v.j_prim[i], v_mom_v.v_self[i], domain_size * sizeof(std::complex<double>));
	}

	//-- Calculate Primary CBF's --//
    // Now lets calculate primary CBF's

	//-- Calculate Secondary CBF's --//
	// First copy coupling z matrices
	// The secondary CBF's are calculated from the primary CBF's
	// Copy the primary CBF's to the secondary CBF's - because LAPACK
	int i_index;
	std::complex<double> minus_one = std::complex<double>(-1.0,0.0);

	for(int i = (num_domains - 1); i > -1; i--)
	{
    	zgetrs_(&tran, &domain_size, &one, v_mom_z.z_self_inv, &domain_size, v_mom_z.z_self_piv, v_mom_v.j_prim[i], &domain_size, &info);
    	
    	std::copy(v_mom_v.j_prim[i], v_mom_v.j_prim[i] + domain_size, v_mom_v.j_cbfm[i]);
    	// std::copy(v_mom_v.j_prim[i], v_mom_v.j_prim[i] + domain_size, arr);
    		
		i_index = num_domains - 1 - i; 

		for(int j = 0; j < (num_domains - 1); j++)
		{
			memcpy(v_mom_v.j_sec[i_index][j], v_mom_v.j_prim[i], domain_size * sizeof(std::complex<double>));
    		
			zgetrs_(&tran, &domain_size, &one, v_mom_z.z_couple_inv[i_index][j], &domain_size, v_mom_z.z_couple_piv[i_index][j],
				    v_mom_v.j_sec[i_index][j], &domain_size, &info);

    	    zscal_(&domain_size, &minus_one, v_mom_v.j_sec[i_index][j], &one);	

    		zgetrs_(&tran, &domain_size, &one, v_mom_z.z_self_inv, &domain_size, v_mom_z.z_self_piv,
    		        v_mom_v.j_sec[i_index][j], &domain_size, &info);

	    	std::copy(v_mom_v.j_sec[i_index][j], v_mom_v.j_sec[i_index][j] + domain_size, v_mom_v.j_cbfm[i_index] + ((j+1) * domain_size));
		}
	}

	//-- Calculate reduced Zcbfm --//
	// zgemm

	//-- Calculate reduced Vcbfm --//

	//-- Calculate reduced Icbfm --//

	//-- Solve Irwg --//






















	file << "---------------------------------------Z_SELF------------------------------------------" << std::endl;
	for(int i = 0; i < domain_size; i++)
	{
		for(int j = 0; j < domain_size; j++)
		{
			file << v_mom_z.z_self[j + i * 4];
		}
		file << std::endl;
	}
	file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;

	file << "---------------------------------------Z_SINV------------------------------------------" << std::endl;
	for(int i = 0; i < domain_size; i++)
	{
		for(int j = 0; j < domain_size; j++)
		{
			file << v_mom_z.z_self_inv[j + i * 4];
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
            file << v_mom_z.z_couple[m][n][j + i * 4];
        }
        file << std::endl;
    }
    file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;
    }}

    for(int m = 0; m < num_domains; m++){
    for(int n = 0; n < (num_domains - 1); n++){	
    file << "--------------------------------------Z_CINV"<<m<<n<<"-----------------------------------------" << std::endl;
    for(int i = 0; i < domain_size; i++)
    {
        for(int j = 0; j < domain_size; j++)
        {
            file << v_mom_z.z_couple_inv[m][n][j + i * 4];
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



	file.close();
}