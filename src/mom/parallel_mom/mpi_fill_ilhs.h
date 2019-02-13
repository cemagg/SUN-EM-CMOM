#ifndef MPI_FILL_ILHS_H
#define MPI_FILL_ILHS_H

#include <complex>
#include <iostream>

extern "C"
{
    void descinit_(int*, int*, int*, int*, int*, int*, int*, int*, int*, int*);
    void pzgetrf_(int*, int*, std::complex<double>*, int*, int*, int*, int*, int*);
    void pzgetrs_(const char*, int*, int*, std::complex<double>*, int*, int*,int* ,int*, std::complex<double>*, int*, int*, int*, int*);

    void Czgesd2d(int, int, int, std::complex<double>*, int, int, int);
    void Czgerv2d(int, int, int, std::complex<double>*, int, int, int);

}

void mpiFillIlhs(std::complex<double> *zmn,
				 std::complex<double> *vrhs,
				 int &num_zmn_rows,
				 int &num_vrhs_rows,
				 int &matrix_size,
				 int &block_size,
				 int &context);

void gatherIlhs(int &context, int &matrix_size, int &block_size, int &proc_row, int &proc_col,
	int &num_vrhs_rows, int &total_proc_rows, std::complex<double> *vrhs, std::complex<double> *gathered_ilhs);

#endif