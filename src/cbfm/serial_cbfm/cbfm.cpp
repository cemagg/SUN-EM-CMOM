#include "cbfm.h"

void performCBFM(std::map<std::string, std::string> &const_map,
				 std::map<int, std::vector<int>> &label_map,
                   std::vector<Triangle> &triangles,
                   std::vector<Edge> &edges,
                   std::vector<Node<double>> &nodes,
                   std::complex<double> *ilhs)
{
	// Define some constants
	int num_domains = label_map.size(); // number of domains

	//-- Create necessary Zmn Matrices --//
	
	// Create struct to store all z matrices and their inverses
	CBFMZMatrices v_mom_z; // Struct storing the vanilla mom Zmn matrices

	// Lets resize all the struct elements to fit the problem
	resizeCBFMZMatricesForEqualDomains(v_mom_z, num_domains, label_map[0][3]);	

	// Lets fill the Z matrices

	// Lets first fill v_mom_z.z_self
	serialFillZmn(v_mom_z.z_self,
				  edges,
				  triangles,
				  nodes,
                  const_map,
                  label_map[0][0],
                  label_map[0][1]+1,
                  label_map[0][0],
                  label_map[0][1]+1);

	std::cout << "---------------------------------------Z_SELF------------------------------------------" << std::endl;
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			std::cout << v_mom_z.z_self[j + i * 4];
		}
		std::cout << std::endl;
	}
	std::cout << "---------------------------------------------------------------------------------------" << std::endl;

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
                  		  	  label_map[i][0],
                  		  	  label_map[i][2],
                  		  	  label_map[j][0],
                  		  	  label_map[j][2]);            
                index++; 
                // std::cout << i  << " <+> " << j << std::endl;
			}
		}	
	}

    for(int m = 0; m < num_domains; m++){
    for(int n = 0; n < (num_domains - 1); n++){	
    std::cout << "--------------------------------------Z_COUP"<<m<<n<<"-----------------------------------------" << std::endl;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            std::cout << v_mom_z.z_couple[m][n][j + i * 4];
        }
        std::cout << std::endl;
    }
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
    }}
	//-- Create necessary Vm vectors --//
	CBFMVectors v_mom_v;

	// Lets resize some of the cbfm vectors
	//resizeCBFMVectorsForEqualDomains(v_mom_v, num_domains, label_map[0][3], false);


	//-- Calculate Primary CBF's --//

	//-- Calculate Secondary CBF's --//

	//-- Calculate reduced Zcbfm --//

	//-- Calculate reduced Vcbfm --//

	//-- Calculate reduced Icbfm --//

	//-- Solve Irwg --//
}