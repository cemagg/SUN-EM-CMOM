#include "fill_ilhs.h"

void serialFillIlhs(std::complex<double> *zmn,
                    std::complex<double> *vrhs,
                    int matrix_size)
{
    int zmn_lda = std::max(1, matrix_size);
    int pivot_vector[matrix_size];
    int info = 0;
    int one = 1;
    char tran = 'N';

    zgetrf_(&matrix_size, &matrix_size, zmn, &zmn_lda, pivot_vector, &info);
    zgetrs_(&tran, &matrix_size, &one, zmn, &matrix_size, pivot_vector, vrhs, &matrix_size, &info);
} 