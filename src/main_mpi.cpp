

//**********************************
#include <iostream>

#include <mpi.h>
// #include "../../lib/args/args.hxx"
//**********************************

int main(int argc, char **argv)
{
	MPI_Init(NULL, NULL);

	// MoMFileReader reader("/home/tameez/pec_plate.mom");

	std::cout << "HELLO WORLD" << std::endl;

	// mpiPerformMoM(reader.const_map,
	// 			  reader.triangles,
	// 			  reader.edges,
	// 			  reader.nodes);
    MPI_Finalize();


	return 0;
}

