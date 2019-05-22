#include "dgfm.h"

void performDGFM(std::map<std::string, std::string> &const_map,
                 std::map<int, Label> &label_map,
                 std::vector<Triangle> &triangles,
                 std::vector<Edge> &edges,
                 std::vector<Node<double>> &nodes,
                 std::vector<Excitation> &excitations,
                 std::complex<double> *ilhs)
{
    int num_domains = label_map.size();
    int domain_size = label_map[0].edge_indices.size();	

    // Allocate Memory
    std::vector<DGFMRow> dgfm_rows;
    DGFMExcitations dgfm_excitations;

    dgfm_rows.resize(num_domains);
    for (int i = 0; i < num_domains; i++)
    {
    	allocateDGFMRowMemory(dgfm_rows[i], num_domains, domain_size);	
    }

    allocateDGFMExcitations(dgfm_excitations, num_domains, domain_size);
   
 	// Calc Excitations
 	fillDGFMExcitations(dgfm_excitations,
					  	num_domains,
					  	domain_size,
					  	const_map,
                 	  	label_map,
                 	  	triangles,
                 	  	edges,
                 	  	nodes,
                 	  	excitations);	

    // Calc Rows
    #pragma omp parallel for
 	for (int i = 0; i < num_domains; i++)
 	{
        //#ifdef OMP
        //std::cout << omp_get_num_threads() << std::endl;
        //#endif
 		calculateDGFMRow(dgfm_rows[i],
					 	 dgfm_excitations,
					 	 i,
					  	 num_domains,
					  	 domain_size,
					  	 const_map,
                 	  	 label_map,
                 	  	 triangles,
                 	  	 edges,
                 	  	 nodes,
                 	  	 excitations);

        std::copy(dgfm_excitations.excitations[i],
                  dgfm_excitations.excitations[i] + domain_size,
                  ilhs + (i * domain_size));
 	}

    // for (int i = 0; i < edges.size(); i++)
    // {
    //     std::cout << ilhs[i] << std::endl;
    // }
}                 

