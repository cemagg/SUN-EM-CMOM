#ifndef WRITE_ILHS_TO_FILE_H
#define WRITE_ILHS_TO_FILE_H

#include <complex>
#include <string>
#include <fstream>

void writeIlhsToFile(std::complex<double> *ilhs,
					 int &matrix_size,
					 std::string file_name);

#endif