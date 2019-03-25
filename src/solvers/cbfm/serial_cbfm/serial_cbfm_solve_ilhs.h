#ifndef SERIAL_CBFM_SOLVE_ILHS_H
#define SERIAL_CBFM_SOLVE_ILHS_H

#include <complex>
#include <vector>

#include "../cbfm_helpers/cbfm_helpers.h"

extern "C"
{
    void zgetrf_(int*, int*, std::complex<double>*, int*, int*, int*);
    void zgetrs_(const char*, int*, int*, std::complex<double>*, int*, int*,
                 std::complex<double>*, int*, int*);

    void zaxpy_(int*, std::complex<double>*, std::complex<double>*, int*,
                std::complex<double>*, int*);
}

void serialSolveIlhsCBFM(CBFMZMatrices &z,
                         CBFMVectors &v,
                         std::vector<SizeMap> sizes,
                         int num_domains,
                         int domain_size,
                         std::complex<double> *ilhs);
#endif
