#ifndef SERIAL_CBF_REDUCER_H
#define SERIAL_CBF_REDUCER_H

#include "../cbfm_helpers/cbfm_helpers.h"

extern "C"
{
    void zgesvd_(const char*, const char*, int*, int*, std::complex<double>*, int*,
                 double*, std::complex<double>*, int*, std::complex<double>*, int*,
                 std::complex<double>*, int*, double*, int*);
}

void serialReduceCBFsWithSVD(CBFMVectors &v,
                             std::vector<SizeMap> &sizes,
                             int &red_size,
                             int num_domains,
                             int domain_size);

#endif
