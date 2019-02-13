#ifndef FILL_ILHS_H
#define FILL_ILHS_H

#include <vector>
#include <complex>

extern "C"
{
    void zgetrf_(int*, int*, std::complex<double>*, int*, int*, int*);
    void zgetrs_(const char*, int*, int*, std::complex<double>*, int*, int*, std::complex<double>*, int*, int*);
}

void serialFillIlhs(std::complex<double> *zmn,
                    std::complex<double> *vrhs,
                    int matrix_size);

#endif