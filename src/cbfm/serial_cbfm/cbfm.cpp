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
                index++; 
			}
		}	
	}

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
	//-- Create necessary Vm vectors --//
	CBFMVectors v_mom_v;

	// Lets resize some of the cbfm vectors
	resizeCBFMVectorsForEqualDomains(v_mom_v, num_domains, domain_size, false);

	// Lets now fill v_self vectors
	for(int i = 0; i < num_domains; i++)
	{
		serialFillVrhs(const_map,
					   triangles,
					   edges,
					   v_mom_v.v_self[i],
					   label_map[i]);
	}

	for(int m = 0; m < num_domains; m++){
    file << "--------------------------------------V_SELF"<<m<<"------------------------------------------" << std::endl;
    for(int i = 0; i < domain_size; i++)
    {
    	file << v_mom_v.v_self[m][i] << std::endl;
    }
    file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;
    }
	//-- Calculate Primary CBF's --//
    // First lets copy the neccessary matrices and vectors because Lapack overwrites them
   	// Lets first copy z_self
   	memcpy(v_mom_z.z_self_inv,
   		   v_mom_z.z_self,
   		   domain_size * domain_size * sizeof(std::complex<double>)); 

   	file << "---------------------------------------Z_SCPY------------------------------------------" << std::endl;
	for(int i = 0; i < domain_size; i++)
	{
		for(int j = 0; j < domain_size; j++)
		{
			file << v_mom_z.z_self_inv[j + i * 4];
		}
		file << std::endl;
	}
	file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;

	// Now lets copy v_self
	for(int i = 0; i < num_domains; i++)
	{
		mempcpy(v_mom_v.j_prim[i], v_mom_v.v_self[i], domain_size * sizeof(std::complex<double>));
	}

	for(int m = 0; m < num_domains; m++){
    file << "--------------------------------------J_PCPY"<<m<<"------------------------------------------" << std::endl;
    for(int i = 0; i < domain_size; i++)
    {
    	file << v_mom_v.j_prim[m][i] << std::endl;
    }
    file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;
    }

    // Now lets calculate primary CBF's

    // First get LU Decomp of z_self
    int z_self_lda = std::max(1, domain_size);
    int info = 0;
    int one = 1;
    char tran = 'N';

    zgetrf_(&domain_size, &domain_size, v_mom_z.z_self_inv, &z_self_lda, v_mom_z.z_self_piv, &info);

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

	// Now lets solve for j_prim
	for(int i = 0; i < num_domains; i++)
	{
    	zgetrs_(&tran, &domain_size, &one, v_mom_z.z_self_inv, &domain_size, v_mom_z.z_self_piv, v_mom_v.j_prim[i], &domain_size, &info);
    }

	for(int m = 0; m < num_domains; m++){
    file << "--------------------------------------J_PRIM"<<m<<"------------------------------------------" << std::endl;
    for(int i = 0; i < domain_size; i++)
    {
    	file << v_mom_v.j_prim[m][i] << std::endl;
    }
    file << "---------------------------------------------------------------------------------------" << std::endl<<std::endl;
    }
	//-- Calculate Secondary CBF's --//

	//-- Calculate reduced Zcbfm --//

	//-- Calculate reduced Vcbfm --//

	//-- Calculate reduced Icbfm --//

	//-- Solve Irwg --//

	file.close();
}