#include "mpi_idgfm.h"

void mpiPerformIDGFM(std::map<std::string, std::string> &const_map,
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

	// Allocate MBFs

	
}