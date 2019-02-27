#ifndef CBFM_H
#define CBFM_H

#include <map>
#include <complex>
#include <vector>
#include <string>

#include "../../data_structures/node.h"
#include "../../data_structures/edge.h"
#include "../../data_structures/triangle.h"

void performCCFM(std::map<std::string, std::string> &const_map,
                   std::vector<Triangle> &triangles,
                   std::vector<Edge> &edges,
                   std::vector<Node<double>> &nodes,
                   std::complex<double> *ilhs); 

#endif