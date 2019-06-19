#include "dgfm_row.h"

#define TIMING
 
#ifdef TIMING
#define INIT_TIMER auto start = std::chrono::high_resolution_clock::now();
#define START_TIMER  start = std::chrono::high_resolution_clock::now();
#define STOP_TIMER(name)  std::cout << "RUNTIME of " << name << ": " << \
    std::chrono::duration_cast<std::chrono::milliseconds>( \
            std::chrono::high_resolution_clock::now()-start \
    ).count() << " ms " << std::endl; 
#else
#define INIT_TIMER
#define START_TIMER
#define STOP_TIMER(name)
#endif

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
					  bool use_threading,
					  std::map<std::string, std::string> &const_map,
                 	  std::map<int, Label> &label_map,
                 	  std::vector<Triangle> &triangles,
                 	  std::vector<Edge> &edges,
                 	  std::vector<Node<double>> &nodes,
                 	  std::vector<Excitation> &excitations)
{
	// INIT_TIMER
	// START_TIMER
	// Get Weights
	int excitation_index = 0;
	calculateDGFMWeights(row, v_vectors, num_domains, excitation_index, domain_index);	
	// STOP_TIMER("Weights")
	// Calcualte Z Matrices
	// START_TIME
	#pragma omp parallel for 
	for (int n = 0; n < num_domains; n++)
	{
		double start = omp_get_wtime();
		// serialFillZmn( 	row.z_matrices[n],
  //       	          	edges,
  //           	      	triangles,
  //               	  	nodes,
  //                  		const_map,
  //                  		label_map[n],
  //                  		label_map[domain_index],
  //                  		true);	
		
		double end = omp_get_wtime();

	   	char processor_name[MPI_MAX_PROCESSOR_NAME];
    	int name_len;
    	MPI_Get_processor_name(processor_name, &name_len);	
		std::cout << processor_name << " tid: " << omp_get_thread_num() <<" on cpu: " << sched_getcpu() << " time: " << end - start << std::endl;
	}
	// STOP_TIMER("Z FILL")
	// Sum Z Matrices
	
	// #pragma omp parallel for 
	// for (int i = 0; i < num_domains; i++)
	// {
	// 	if (i != domain_index)
	// 	{
	// 		for (int j = 0; j < (domain_size * domain_size); j++)
	// 		{
	// 			row.z_matrices[domain_index][j] += row.dgfm_weights[i] * row.z_matrices[i][j]; 
	// 		}
	// 	}
	// }
	// STOP_TIMER("ZACT")
	// std::complex<double> sum = std::complex<double>(0.0, 0.0);

	// #pragma omp parallel for
	// for (int i = 0; i < (domain_size * domain_size); i++)
	// {
	// 	for (int j = 0; j < num_domains; j++)
	// 	{
	// 		sum += row.dgfm_weights[j] * row.z_matrices[j][i];
	// 	}

	// 	row.z_matrices[domain_index][i] = sum;
	// 	sum = std::complex<double>(0.0, 0.0);
	// }

	// Solve for I
	
	// serialFillIlhs(row.z_matrices[domain_index],
 //                   v_vectors.excitations[domain_index],
 //                   domain_size);	
	// STOP_TIMER("LUDC")
}
