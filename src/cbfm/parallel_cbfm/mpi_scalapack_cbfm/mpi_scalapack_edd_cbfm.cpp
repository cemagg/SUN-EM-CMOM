#include "mpi_scalapack_edd_cbfm.h" 

void mpiScalapackEDDCBFM(std::map<std::string, std::string> &const_map,
                         std::map<int, Label> &label_map,
                         std::vector<Triangle> &triangles,
                         std::vector<Edge> &edges,
                         std::vector<Node<double>> &nodes,
                         std::complex<double> *ilhs,
                         CBLACSConfig &config)
{
    // Fill Zself and Vself

    // Fill CBF's

    // SVD

    // Reduce Z and V

    // Solve for ilhs
}
