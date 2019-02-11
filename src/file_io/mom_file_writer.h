#ifndef MOM_FILE_WRITER_H
#define MOM_FILE_WRITER_H

#include <complex>
#include <string>
#include <fstream>

void writeIlhsToFile(std::complex<double> *ilhs,
					 int &matrix_size,
					 std::string file_name);

#endif