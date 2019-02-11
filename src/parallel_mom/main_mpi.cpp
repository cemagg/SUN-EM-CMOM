

//**********************************
#include <iostream>

#include "mpi_fill_vrhs.h"
#include "../file_io/mom_file_reader.h"
#include "mpi_mom.h"
#include <mpi.h>
// #include "../../lib/args/args.hxx"
//**********************************

int main(int argc, char **argv)
{
	// args::ArgumentParser parser("This is a test.", "this goes after options");
	MoMFileReader reader("/home/tameez/pec_plate.mom");

	std::cout << "HELLO WORLD" << std::endl;

	mpiPerformMoM(reader.const_map,
				  reader.triangles,
				  reader.edges,
				  reader.nodes);
	//std::complex<double> *vrhs = new std::complex<double>[8]();

	return 0;
}

