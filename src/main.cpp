#include <iostream>


//**********************************
#include <complex>
#include <iostream>
#include <vector>
#include <string>

#include "../lib/args/args.hxx"
#include "file_io/mom_file_reader.h"
#include "file_io/mom_file_writer.h"
#include "mom/serial_mom/mom.h"
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

    if(cbfm)
    {
        std::cout << "TBD" << std::endl;
    }
    else
    {
        MoMFileReader reader(args::get(file_name_arg));
        
        std::complex<double> *ilhs = new std::complex<double>[reader.edges.size()];

        mpiPerformMoM(reader.const_map,
                        reader.triangles,
                        reader.edges,
                        reader.nodes,
                        ilhs);

        writeIlhsToFile(ilhs,
                        reader.edges.size(),
                        args::get(file_name_arg));
        
        delete ilhs;

        std::cout << "SOLVER COMPLETE" << std::endl;
    }
    return 0;
}
