#include <iostream>


//**********************************
#include <complex>
#include <iostream>
#include <vector>
#include <string>

#include "../file_io/mom_file_reader.h"
#include "../data_structures/node.h"
#include "fill_vrhs.h"
#include "fill_zmn.h"
#include "../../lib/args/args.hxx"
//**********************************

int main(int argc, char **argv)
{
//    MoMFileReader reader("C:\\Users\\Tameez\\Documents\\git\\CMoM\\src\\build-mom-Desktop_Qt_5_12_0_MinGW_64_bit-Debug\\debug\\pec_plate.mom");

	args::ArgumentParser parser("CMoM: C++ Method of Moments Solver", "");
	args::HelpFlag help(parser, "help", "Display this menu", {'h', "help"});
	args::Positional<std::string> file_name_arg(parser, "file_path", "The path to the .mom file");
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

    std::cout << args::get(file_name_arg) << std::endl;
    return 0;
}
