#include "mpi_fill_ilhs.h"

void mpiFillIlhs(std::complex<double> *zmn,
				 std::complex<double> *vrhs,
				 int &num_zmn_rows,
				 int &num_vrhs_rows,
				 int &matrix_size,
				 int &block_size,
				 int &context)
{
	int zero = 0;
	int one = 1;
	int info = 0;

	int zmn_lda = std::max(1, num_zmn_rows);
	int zmn_desc[9];

	descinit_(zmn_desc, &matrix_size, &matrix_size, &block_size, &block_size, &zero, &zero, &context, &zmn_lda, &info);

	int vrhs_lda = std::max(1, num_vrhs_rows);
	int vrhs_desc[9];

	descinit_(vrhs_desc, &matrix_size, &one, &block_size, &block_size, &zero, &zero, &context, &vrhs_lda, &info);

	int local_pivot[num_zmn_rows * block_size];
    pzgetrf_(&matrix_size, &matrix_size, zmn, &one, &one, zmn_desc, local_pivot, &info); 

    // Lets solve for I
    pzgetrs_("N", &matrix_size, &one, zmn, &one, &one, zmn_desc, local_pivot, vrhs, &one, &one, vrhs_desc, &info);


}