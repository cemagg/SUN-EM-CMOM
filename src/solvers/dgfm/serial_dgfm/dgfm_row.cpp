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
	#ifndef PARALLEL
	auto start = std::chrono::steady_clock::now();
	#endif

	#ifndef PARALLEL	
	int max_gang_size = 1024;
	int n_vectors = ((domain_size * domain_size / 32) + 1) * 32;
	int n_workers = (max_gang_size / n_vectors > 32) ? 32 : (max_gang_size / n_vectors);

	#pragma acc parallel loop device_type(nvidia) vector_length(n_vectors) gang worker num_workers(n_workers)
	#endif
	for (int i = 0; i < num_domains; i++)
	{
		if (i != domain_index)
		{
			#ifndef PARALLEL
			#pragma omp parallel for
			#pragma acc loop device_type(nvidia) vector
			#endif
			for (int j = 0; j < (domain_size * domain_size); j++)
			{
				row.z_matrices[domain_index][j] += row.dgfm_weights[i] * row.z_matrices[i][j]; 
			}
		}
	}

	
	// #pragma acc parallel loop device_type(nvidia) vector_length(n_vectors) gang worker num_workers(n_workers)
	// for (int i = 0; i < (domain_size * domain_size); i++)
	// {
	// 	std::complex<double> sum = std::complex<double>(0.0, 0.0);

	// 	#pragma acc loop reduction(+:sum) device_type(nvidia) vector
	// 	for (int j = 0; j < num_domains; j++)
	// 	{
	// 		if (j != domain_index)
	// 		{
	// 			sum += row.dgfm_weights[j] * row.z_matrices[j][i];
	// 		}
	// 	}
	// 	row.z_matrices[domain_index][i] = sum;	
	// }

	#ifndef PARALLEL
	auto end = std::chrono::steady_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl; 
	#endif

	// Solve for I

	serialFillIlhs(row.z_matrices[domain_index],
                   v_vectors.excitations[domain_index],
                   domain_size);	
}
