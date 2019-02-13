#include <iostream>
#include <mpi.h>
#include <complex>

#include "../lib/args/args.hxx"
#include "file_io/mom_file_reader.h"
#include "file_io/mom_file_writer.h"
#include "mom/parallel_mom/mpi_mom.h"

int main(int argc, char **argv)
{
	MPI_Init(NULL, NULL);

	int size;
    int rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	args::ArgumentParser parser("CMoM: C++ Method of Moments Solver", "");
	args::HelpFlag help(parser, "help", "Display this menu", {'h', "help"});
	args::Positional<std::string> file_name_arg(parser, "file_path", "The path to the .mom file");
	args::Group group(parser, "");
	args::Flag cbfm(group, "cbfm", "TBD", {"cbfm"});
	
	try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (const args::Completion& e)
    {
        std::cout << e.what();
        return 0;
    }
    catch (const args::Help&)
    {
        std::cout << parser;
        return 0;
    }
    catch (const args::ParseError& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    if(cbfm)
    {
    	std::cout << "TBD" << std::endl;
    }
    else
    {
    	MoMFileReader reader(args::get(file_name_arg));

    	std::complex<double> *ilhs;

    	if(rank == 0)
    	{
    		ilhs = new std::complex<double>[reader.edges.size()];
    	}

    	mpiPerformMoM(reader.const_map, reader.triangles, reader.edges, reader.nodes, ilhs);

    	if(rank == 0)
    	{
    		writeIlhsToFile(ilhs, reader.edges.size(), args::get(file_name_arg));	
    		delete ilhs;
    		std::cout << "SOLVER COMPLETE" << std::endl;
    	}
	}

    MPI_Finalize();


	return 0;
}

