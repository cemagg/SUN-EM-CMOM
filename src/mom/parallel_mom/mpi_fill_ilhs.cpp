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
    pzgetrs_("T", &matrix_size, &one, zmn, &one, &one, zmn_desc, local_pivot, vrhs, &one, &one, vrhs_desc, &info);


}

void gatherIlhs(int &context, int &matrix_size, int &block_size, int &proc_row, int &proc_col,
	int &num_vrhs_rows, int &total_proc_rows, std::complex<double> *vrhs, std::complex<double> *gathered_ilhs)
{
    int send_row = 0;
    int vrhs_local_index = 0;
    for(int i = 0; i < matrix_size; i+= block_size)
    {
        int num_rows = block_size;
        if(matrix_size - i < block_size)
        {
            num_rows = matrix_size - i;
        }

        if(send_row >= total_proc_rows)
        {
            send_row = 0;
        }

        if(proc_row == send_row && proc_col == 0)
        {
            Czgesd2d(context, num_rows, 1, vrhs + vrhs_local_index , matrix_size, 0, 0);

            vrhs_local_index += block_size;
        }
        if(proc_row == 0 && proc_col == 0)
        {
            Czgerv2d(context, num_rows, 1, gathered_ilhs + i, num_vrhs_rows, send_row, 0);   
        }
        send_row++;
    }
}