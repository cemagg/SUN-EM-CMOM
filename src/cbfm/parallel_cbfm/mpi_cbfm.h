#ifndef MPI_CBFM_H
#define MPI_CBFM_H

void mpiPerformCBFM(std::map<std::string, std::string> &const_map,
                 std::map<int, Label> &label_map,
                 std::vector<Triangle> &triangles,
                 std::vector<Edge> &edges,
                 std::vector<Node<double>> &nodes,
                 std::complex<double> *ilhs);

#endif