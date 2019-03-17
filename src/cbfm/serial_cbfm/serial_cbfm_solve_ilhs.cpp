#include "serial_cbfm_solve_ilhs.h"
#include <fstream>

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

    std::ofstream f;
    f.open("c.txt");
    
    for (int i = 0; i < sum_cbfs; i++)
    {
        for (int j = 0; j < sum_cbfs; j++)
        {
            f << z.z_red_concat[j + i * sum_cbfs];
        }
        f << std::endl;
    }
        f << std::endl;
        f << std::endl;
        f << std::endl;
        f << std::endl;
    zgetrf_(&sum_cbfs, &sum_cbfs, z.z_red_concat, &sum_cbfs, z_red_piv, &info);
    zgetrs_(&norm, &sum_cbfs, &one, z.z_red_concat, &sum_cbfs, z_red_piv,
            v.v_red_concat, &sum_cbfs, &info);
    for (int i = 0; i < sum_cbfs; i++)
    {
        for (int j = 0; j < sum_cbfs; j++)
        {
            f << z.z_red_concat[j + i * sum_cbfs];
        }
        f << std::endl;
    }
    f.close();
    //Print ICBFM
    //  std::cout << std::endl;
    //  std::cout << std::endl;
    //  std::cout << std::endl;
    std::cout << sum_cbfs << std::endl;
    std::cout << "=====================" << std::endl;
    for (int i = 0; i < sum_cbfs; i++)
    {
        std::cout << v.v_red_concat[i] << std::endl;
    }
    std::cout << "=====================" << std::endl;
    
    int rwg_pos = 0;
    int prim_index = 0;
    int sec_index  = 0;
    
    for (int i = 0; i < num_domains; i++)
    {
               
        for (int j = 0; j < sizes[i].n_cbfs; j++)
        {
            if (j == i)
            {
                zaxpy_(&domain_size, &v.v_red_concat[prim_index], v.j_prim[i], &one,
                       ilhs + rwg_pos, &one);
                prim_index += sizes[i].n_cbfs;
            }
            else
            {
                zaxpy_(&domain_size, &v.v_red_concat[sec_index], v.j_sec[i][j], &one,
                       ilhs + rwg_pos, &one);
            }
            sec_index++;
        }
        rwg_pos += domain_size;
    }
    
}
