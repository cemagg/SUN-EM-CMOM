#include "serial_generate_cbfs.h"

void generateEDDCBFs(CBFMZMatrices &z,
                     CBFMVectors &v,
                     int num_domains,
                     int domain_size,
                     std::map<std::string, std::string> &const_map,
                     std::map<int, Label> &label_map,
                     std::vector<Triangle> &triangles,
                     std::vector<Edge> &edges,
                     std::vector<Node<double>> &nodes)
{

    // Values needed for LAPACK
    int info = 0;
    int one = 1;
    char tran = 'T';
    std::complex<double> c_minus_one = std::complex<double>(-1.0, 0.0);
    std::complex<double> c_zero = std::complex<double>(0.0, 0.0);
    
    // LU-decomp of Zself
    zgetrf_(&domain_size, &domain_size, z.z_self_inv, &domain_size, z.z_self_piv, &info);
    
    std::vector<int> copy_index(num_domains, 1);

    for (int i = 0; i < num_domains; i++)
    {
        zgetrs_(&tran, &domain_size, &one, z.z_self_inv, &domain_size, z.z_self_piv,
                v.j_prim[i], &domain_size, &info);

        // Copy primary CBF to concatenated CBF matrix
        std::copy(v.j_prim[i], v.j_prim[i] + domain_size, v.j_cbfm[i]);

        for (int j = 0; j < num_domains; j++)
        {
            if (j != i)
            {
                // Fill coupling Z matrices
                serialFillZmn(z.z_couple[j][i], edges, triangles, nodes, const_map,
                              label_map[j], label_map[i], true);
                
                
                // -Z_couple * J_prim
                zgemv_(&tran, &domain_size, &domain_size, &c_minus_one, z.z_couple[j][i],
                       &domain_size, v.j_prim[i], &one, &c_zero, v.j_sec[j][i], &one);

                // Calculate secondary CBF's
                // Z_self_inv * (-Z_couple * J_prim)
                zgetrs_(&tran, &domain_size, &one, z.z_self_inv, &domain_size,
                        z.z_self_piv, v.j_sec[j][i], &domain_size, &info);

                // Copy secondary CBF's to concatenated CBF matrix
                std::copy(v.j_sec[j][i], v.j_sec[j][i] + domain_size,
                          v.j_cbfm[j] + (copy_index[j] * domain_size));

                copy_index[j]++;
            }
        }
    }
}
