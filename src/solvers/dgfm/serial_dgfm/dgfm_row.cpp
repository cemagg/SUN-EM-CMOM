#include "dgfm_row.h"

void fillDGFMExcitations(DGFMExcitations &v_vectors,
					  	int num_domains,
					  	int domain_size,
					  	std::map<std::string, std::string> &const_map,
                 	  	std::map<int, Label> &label_map,
                 	  	std::vector<Triangle> &triangles,
                 	  	std::vector<Edge> &edges,
                 	  	std::vector<Node<double>> &nodes,
                 	  	std::vector<Excitation> &excitations)
{
    for (int i = 0; i < num_domains; i++)
	{
		serialFillVrhs(	const_map,
       		            triangles,
           		        edges,
               		    excitations,
                   		v_vectors.excitations[i],
                   		i,
                   		label_map[i],
                   		true);	
	}	
}

void calculateDGFMRow(DGFMRow &row,
					  DGFMExcitations &v_vectors,
					  int domain_index,
					  int num_domains,
					  int domain_size,
					  std::map<std::string, std::string> &const_map,
                 	  std::map<int, Label> &label_map,
                 	  std::vector<Triangle> &triangles,
                 	  std::vector<Edge> &edges,
                 	  std::vector<Node<double>> &nodes,
                 	  std::vector<Excitation> &excitations)
{
	// Get Weights
	int excitation_index = 0;
	calculateDGFMWeights(row, v_vectors, num_domains, excitation_index, domain_index);	

	// Calcualte Z Matrices
	#pragma omp parallel for
	for (int n = 0; n < num_domains; n++)
	{
		serialFillZmn( 	row.z_matrices[n],
        	          	edges,
            	      	triangles,
                	  	nodes,
                   		const_map,
                   		label_map[n],
                   		label_map[domain_index],
                   		true);	
	}

	// Sum Z Matrices
	#pragma omp parallel for
	for (int i = 0; i < num_domains; i++)
	{
		if (i != domain_index)
		{
			for (int j = 0; j < (domain_size * domain_size); j++)
			{
				row.z_matrices[domain_index][j] += row.dgfm_weights[i] * row.z_matrices[i][j]; 
			}
		}
	}

	// Solve for I

	serialFillIlhs(row.z_matrices[domain_index],
                   v_vectors.excitations[domain_index],
                   domain_size);	
}
