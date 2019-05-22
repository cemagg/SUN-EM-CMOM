#ifndef SERIAL_Z_V_REDUCER_H
#define SERIAL_Z_V_REDUCER_H

#include <complex>
#include <vector>

#include "../cbfm_helpers/cbfm_helpers.h"

extern "C"
{
    void zgemm_(const char*, const char*, int*, int*, int*, std::complex<double>*,
                std::complex<double>*, int*, std::complex<double>*, int*,
                std::complex<double>*, std::complex<double>*, int*);

    void zgemv_(const char*, int*, int*, std::complex<double>*, std::complex<double>*,
                int*, std::complex<double>*, int*, std::complex<double>*,
                std::complex<double>*, int*);
}

void serialZVReducerForEDD(CBFMZMatrices &z,
                           CBFMVectors &v,
                           std::vector<SizeMap> sizes,
                           int num_domains,
                           int domain_size,
                           int red_size);


#endif

