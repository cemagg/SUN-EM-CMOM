#include "mpi_dgfm.h"

void mpiPerformDGFM(std::map<std::string, std::string> &const_map,
					std::map<int, Label> &label_map,
					std::vector<Triangle> &triangles,
					std::vector<Edge> &edges,
					std::vector<Node<double>> &nodes,
					std::vector<Excitation> &excitations,
					std::complex<double> *ilhs)
{
	int size;
	int rank;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); 	

	// Get num domains and domain size
	int num_domains = label_map.size();
	int domain_size = label_map[0].edge_indices.size();	 

	// Allocate Work
	int *displs = new int[size];
	displs[0] = 0;
	
	for (int i = 1; i < size; i++)
	{
		displs[i] = displs[i - 1] + ((num_domains + i) / size);
		
	}

	int local_num_rows;

	if (rank == (size - 1))
	{
		local_num_rows = num_domains - displs[rank];
	}
	else
	{
		local_num_rows = displs[rank + 1] - displs[rank];
	}

	// Allocate Memory

	std::vector<DGFMRow> dgfm_rows;
	DGFMExcitations dgfm_excitations;

	dgfm_rows.resize(local_num_rows);
	for (int i = 0; i < local_num_rows; i++)
	{
		allocateDGFMRowMemory(dgfm_rows[i], num_domains, domain_size);	
	}

	allocateDGFMExcitations(dgfm_excitations, num_domains, domain_size);

	std::complex<double> *local_ilhs = new std::complex<double>[local_num_rows * domain_size];

	// Do the work
	
	fillDGFMExcitations( dgfm_excitations,
						 num_domains,
						 domain_size,
						 const_map,
						 label_map,
						 triangles,
						 edges,
						 nodes,
						 excitations);

	int start_index = displs[rank];
	int end_index = displs[rank] + local_num_rows;

	#pragma omp parallel for
	for (int i = start_index; i < end_index; i++)
	{
	  calculateDGFMRow(dgfm_rows[i - start_index],
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
				  local_ilhs + ((i - start_index) * domain_size));

	}

	// Gather to proc 0
	int *recv_counts;

	if (rank == 0)
	{
		recv_counts = new int[size];

		for (int i = 0; i < size; i++)
		{
			if (i == (size - 1))
			{
				recv_counts[i] = (num_domains - displs[i]) * domain_size;
			}
			else
			{
				recv_counts[i] = (displs[i + 1] - displs[i]) * domain_size;
			}
			displs[i] = displs[i] * domain_size;
		}
	}

	MPI_Gatherv( local_ilhs, 
			     local_num_rows * domain_size,
			     MPI_CXX_DOUBLE_COMPLEX,
			     ilhs,
			     recv_counts,
			     displs,
			     MPI_CXX_DOUBLE_COMPLEX,
			     0,
			     MPI_COMM_WORLD);
}

