#ifndef MPI_FILL_ILHS_H
#define MPI_FILL_ILHS_H

#include <complex>
#include <iostream>

extern "C"
{
    void descinit_(int*, int*, int*, int*, int*, int*, int*, int*, int*, int*);
    void pzgetrf_(int*, int*, std::complex<double>*, int*, int*, int*, int*, int*);
    void pzgetrs_(const char*, int*, int*, std::complex<double>*, int*, int*,int* ,int*, std::complex<double>*, int*, int*, int*, int*);

}

void mpiFillIlhs(std::complex<double> *zmn,
				 std::complex<double> *vrhs,
				 int &num_zmn_rows,
				 int &num_vrhs_rows,
				 int &matrix_size,
				 int &block_size,
				 int &context);


#endif