#include "serial_edd_cbfm.h"

// This function calculates the CBFM for identical non-connected elements
void serialEDDCBFM(std::map<std::string, std::string> &const_map,
                   std::map<int, Label> &label_map,
                   std::vector<Triangle> &triangles,
                   std::vector<Edge> &edges,
                   std::vector<Node<double>> &nodes)
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

    // Do SVD if required, else skip
    std::vector<SizeMap> cbf_sizes;
    cbf_sizes.resize(num_domains);
    
    bool svd = false;
    if (svd)
    {
        
    }
    else
    {
        for (int i = 0; i < num_domains; i++)
        {
            cbf_sizes[i].n_cbfs = num_domains;
        }
    }

    // Calculare Vred and Zred
    resizeCBFMZredForEDD(z_mat, v_vec, cbf_sizes, domain_size);
    

    // Calculate Icbfm

    // Map CBF's back to Irwg

}
