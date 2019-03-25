#include "serial_cbfm_solve_ilhs.h"

void serialSolveIlhsCBFM(CBFMZMatrices &z,
                         CBFMVectors &v,
                         std::vector<SizeMap> sizes,
                         int num_domains,
                         int domain_size,
                         std::complex<double> *ilhs)
{
    int sum_cbfs = 0;
    
    for (int i = 0; i < num_domains; i++)
    {
        sum_cbfs += sizes[i].n_cbfs;
    }

    int z_red_piv[sum_cbfs];

    int info;
    int one = 1;
    char norm = 'N';
    char tran = 'T';

    
    zgetrf_(&sum_cbfs, &sum_cbfs, z.z_red_concat, &sum_cbfs, z_red_piv, &info);
    zgetrs_(&norm, &sum_cbfs, &one, z.z_red_concat, &sum_cbfs, z_red_piv,
            v.v_red_concat, &sum_cbfs, &info);
    
    int rwg_pos = 0;
    int index = 0;

    for (int i = 0; i < num_domains; i++)
    {
               
        for (int j = 0; j < sizes[i].n_cbfs; j++)
        {
            zaxpy_(&domain_size, &v.v_red_concat[index], v.j_cbfm[i] + (j * domain_size),
                   &one, ilhs + rwg_pos, &one);

            index++;
        }
        rwg_pos += domain_size;
    }
    
}
