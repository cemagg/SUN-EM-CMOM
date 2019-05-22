#ifndef DGFM_H
#define DGFM_H

#include <complex>
#include <vector>
#include <map>
#include <iostream>

#include "../../../data_structures/node.h"
#include "../../../data_structures/edge.h"
#include "../../../data_structures/triangle.h"
#include "../../../data_structures/label.h"

#include "dgfm_row.h"

#include "../dgfm_helpers/dgfm_helpers.h"

#ifdef OMP
#include <omp.h>
#endif

void performDGFM(std::map<std::string, std::string> &const_map,
                 std::map<int, Label> &label_map,
                 std::vector<Triangle> &triangles,
                 std::vector<Edge> &edges,
                 std::vector<Node<double>> &nodes,
                 std::vector<Excitation> &excitations,
                 std::complex<double> *ilhs);

#endif
