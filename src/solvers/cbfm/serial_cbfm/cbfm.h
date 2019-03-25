#ifndef CBFM_H
#define CBFM_H

#include <complex>

#include "serial_edd_cbfm.h"

void performCBFM(std::map<std::string, std::string> &const_map,
                 std::map<int, Label> &label_map,
                 std::vector<Triangle> &triangles,
                 std::vector<Edge> &edges,
                 std::vector<Node<double>> &nodes,
                 std::complex<double> *ilhs);

#endif
