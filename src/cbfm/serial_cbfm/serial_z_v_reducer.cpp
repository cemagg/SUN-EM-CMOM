#include "serial_z_v_reducer.h"

void serialZVReducerForEDD(CBFMZMatrices &z,
                           CBFMVectors &v,
                           std::vector<SizeMap> sizes,
                           int num_domains,
                           int domain_size)
{

    char tran = 'T';
    char norm = 'N';
    int one = 1;
    std::complex<double> c_one  = std::complex<double>(1.0, 0.0);
    std::complex<double> c_zero = std::complex<double>(0.0, 0.0);

    std::complex<double> *c_temp;
    
    int disp_index;
    int v_index = 0;

    for (int i = 0; i < num_domains; i++)
    {
        disp_index = i * sizes[i].n_cbfs;

        zgemv_(&tran, &domain_size, &sizes[i].n_cbfs, &c_one, v.j_cbfm[i],
               &domain_size, v.v_self[i], &one, &c_zero, v.v_red_concat + v_index, &one);
        v_index += sizes[i].n_cbfs;

        for (int j = 0; j < num_domains; j++)
        {
            c_temp = new std::complex<double>[domain_size * sizes[i].n_cbfs];

            if (j == i)
            {
                zgemm_(&tran, &norm, &domain_size, &sizes[i].n_cbfs, &domain_size,
                       &c_one, z.z_self, &domain_size, v.j_cbfm[i], &domain_size,
                       &c_zero, c_temp, &domain_size);
            }
            else
            {
                zgemm_(&tran, &norm, &domain_size, &sizes[i].n_cbfs, &domain_size,
                       &c_one, z.z_couple[i][j], &domain_size, v.j_cbfm[i], &domain_size,
                       &c_zero, c_temp, &domain_size);
            }

            zgemm_(&tran, &norm, &sizes[i].n_cbfs, &sizes[i].n_cbfs, &domain_size,
                   &c_one, v.j_cbfm[i], &domain_size, c_temp, &domain_size, &c_zero,
                   z.z_red[i][j], &sizes[i].n_cbfs);
            
            delete [] c_temp;
        }

        for (int k = 0; k < sizes[i].n_cbfs; k++)
        {
            std::copy(z.z_red[i][j] + (k * sizes[i].n_cbfs),
                      z.z_red[i][j] + ((k + 1) * sizes[i].n_cbfs),
                      z.z_red_concat + disp_index);
            disp_index += sizes[i].n_cbfs * sizes[i].n_cbfs;
        }
    }
}