#include "fill_self_matrices_vectors.h"

void fillZVSelfForEDD(CBFMZMatrices &z,
                      CBFMVectors &v,
                      int num_domains,
                      int domain_size,
                      std::map<std::string, std::string> &const_map,
                      std::map<int, Label> &label_map,
                      std::vector<Triangle> &triangles,
                      std::vector<Edge> &edges,
                      std::vector<Excitation> &excitations,
                      std::vector<Node<double>> &nodes)
{
    serialFillZmn(z.z_self,
                  edges,
                  triangles,
                  nodes,
                  const_map,
                  label_map[0],
                  label_map[0],
                  false);

    // Copy to inv for future LU-decomp because of LAPACK overwrite
    std::copy(z.z_self, z.z_self + (domain_size * domain_size), z.z_self_inv);

    for (int i = 0; i < num_domains; i++)
    {
        serialFillVrhs(const_map, triangles, edges, excitations, v.v_self[i], label_map[i]);

        // Copy to j_prim for future calculations because of LAPACK overwrite
        std::copy(v.v_self[i], v.v_self[i] + domain_size, v.j_prim[i]);
    }
}
