//**********************************
#include <complex>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

#include "../lib/args/args.hxx"
#include "file_io/mom_file_reader.h"
#include "file_io/mom_file_writer.h"

#ifndef PARALLEL
#include "mom/serial_mom/mom.h"
#include "cbfm/serial_cbfm/cbfm.h"
#endif

#ifdef PARALLEL
#include "mom/parallel_mom/mpi_mom.h"
#include <mpi.h>
#endif
//**********************************

int main(int argc, char **argv)
{

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

    #ifdef PARALLEL
    MPI_Init(NULL, NULL);
    int size;
    int rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    #endif

    MoMFileReader reader(args::get(file_name_arg));
    std::complex<double> *ilhs; 

    if(cbfm)
    {
        std::cout << "TBD" << std::endl;
        performCBFM(reader.const_map,
                    reader.label_map,
                    reader.triangles,
                    reader.edges,
                    reader.nodes,
                    ilhs);  
    }
    else
    {
        #ifndef PARALLEL
        ilhs = new std::complex<double>[reader.edges.size()];
        int rank = 0;
        #endif

        #ifdef PARALLEL
        if(rank == 0)
        {
            ilhs = new std::complex<double>[reader.edges.size()];
        }
        #endif

        mpiPerformMoM(reader.const_map,
                        reader.triangles,
                        reader.edges,
                        reader.nodes,
                        ilhs);      

        if(rank == 0)
        {
            writeIlhsToFile(ilhs, reader.edges.size(), args::get(file_name_arg));   
            std::cout << "SOLVER COMPLETE" << std::endl;
            delete ilhs;
        } 

    }


    #ifdef PARALLEL
    MPI_Finalize();
    #endif

    return 0;
}
