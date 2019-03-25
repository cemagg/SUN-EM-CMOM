#ifndef MPI_SCALAPACK_EDD_CBFM_H
#define MPI_SCALAPACK_EDD_CBFM_H

#include <map>
#include <complex>
#include <vector>

#include "../../../data_structures/node.h"
#include "../../../data_structures/edge.h"
#include "../../../data_structures/triangle.h"
#include "../../../data_structures/label.h"
#include "../../../data_structures/cblacs_config.h"

#include "../../cbfm_helpers/cbfm_helpers.h"

void mpiScalapackEDDCBFM(std::map<std::string, std::string> &const_map,
                         std::map<int, Label> &label_map,
                         std::vector<Triangle> &triangles,
                         std::vector<Edge> &edges,
                         std::vector<Node<double>> &nodes,
                         std::complex<double> *ilhs);

#endif
