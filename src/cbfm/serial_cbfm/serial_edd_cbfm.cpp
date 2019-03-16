#include "serial_edd_cbfm.h"

// This function calculates the CBFM for identical non-connected elements
void serialEDDCBFM(std::map<std::string, std::string> &const_map,
                   std::map<int, Label> &label_map,
                   std::vector<Triangle> &triangles,
                   std::vector<Edge> &edges,
                   std::vector<Node<double>> &nodes,
                   std::complex<double> *ilhs)
{
    // init
    int num_domains = label_map.size();
    int domain_size = label_map[0].edge_indices.size();
    
    // Declare Structs
    CBFMZMatrices z_mat;
    CBFMVectors v_vec;

    
    resizeCBFMZMatricesForEDD(z_mat, num_domains, domain_size);
    resizeCBFMVAndCBFsForEDD(v_vec, num_domains, domain_size);

    // Fill Zself and Vself
    fillZVSelfForEDD(z_mat,
                     v_vec,
                     num_domains,
                     domain_size,
                     const_map,
                     label_map,
                     triangles,
                     edges,
                     nodes);


    // Calculate CBFprim CBFsec and Zcouple
    generateEDDCBFs(z_mat,
                    v_vec,
                    num_domains,
                    domain_size,
                    const_map,
                    label_map,
                    triangles,
                    edges,
                    nodes);

    // Print prim
    for (int i = 0; i < num_domains; i++)
    {
        std::cout << "JPRIM " << i << std::endl;
        for (int j = 0; j < domain_size; j++)
        {
            std::cout << v_vec.j_prim[i][j] << std::endl;
        }
        std::cout << "+++++++++++++++++++++++++++++++" << std::endl;
    }
    //Print sec
    for (int i = 0; i < num_domains; i++)
    {
        for (int j = 0; j < num_domains; j++)
        {
            if (j != i)
            {
                std::cout << "JSEC " << i <<" " << j << std::endl; 
                for (int k = 0; k < domain_size; k++)
                {
                    std::cout << v_vec.j_sec[i][j][k] << std::endl;
                }
                std::cout << "+++++++++++++++++++++" << std::endl;
            }
        }
     }
    
    
    // Do SVD if required, else skip
    std::vector<SizeMap> cbf_sizes;
    cbf_sizes.resize(num_domains);
    int red_size = 0;
    
    bool svd = false;
    if (svd)
    {
        
    }
    else
    {
        for (int i = 0; i < num_domains; i++)
        {
            cbf_sizes[i].n_cbfs = num_domains;
            red_size += num_domains;
        }
    }

    // Calculare Vred and Zred
    resizeCBFMZredForEDD(z_mat, v_vec, cbf_sizes, domain_size);
    serialZVReducerForEDD(z_mat, v_vec, cbf_sizes, num_domains, domain_size, red_size);

    // Print Vredconcat
    // for (int i = 0 ; i < red_size; i++)
    // {
    //     std::cout << v_vec.v_red_concat[i] << std::endl;
    // }
    // Print Zred
    // for (int i = 0; i < num_domains; i++)
    // {
    //     for (int j = 0; j < num_domains; j++)
    //     {
    //         std::cout << i << " " << j << std::endl;
    //         for (int m = 0; m < num_domains; m++)
    //         {
    //             for (int n = 0; n < num_domains; n++)
    //             {
    //                 std::cout << z_mat.z_red[i][j][m + n * num_domains];
    //             }
    //             std::cout << std::endl;
    //         }
    //         std::cout << "++++++++++++++++++++++" << std::endl;
    //     }
    // }
    
    // Calculate Icbfm
    serialSolveIlhsCBFM(z_mat,
                        v_vec,
                        cbf_sizes,
                        num_domains,
                        domain_size,
                        ilhs);

    // Map CBF's back to Irwg

}
