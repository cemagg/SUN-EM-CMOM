#ifndef FILL_SELF_MATRICES_VECTORS_H
#define FILL_SELF_MATRICES_VECTORS_H

#include <map>
#include <vector>
#include <complex>
#include <algorithm>

#include "../cbfm_helpers/cbfm_helpers.h"

#include "../../../data_structures/node.h"
#include "../../../data_structures/edge.h"
#include "../../../data_structures/triangle.h"
#include "../../../data_structures/label.h"

#include "../../mom/serial_mom/fill_vrhs.h"
#include "../../mom/serial_mom/fill_zmn.h"

void fillZVSelfForEDD(CBFMZMatrices &z,
                      CBFMVectors &v,
                      int num_domains,
                      int domain_size,
                      std::map<std::string, std::string> &const_map,
                      std::map<int, Label> &label_map,
                      std::vector<Triangle> &triangles,
                      std::vector<Edge> &edges,
                      std::vector<Excitation> &excitations,
                      std::vector<Node<double>> &nodes);

#endif

