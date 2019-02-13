#ifndef MOM_H
#define MOM_H

#include <vector>
#include <complex>

#include "fill_vrhs.h"
#include "fill_zmn.h"
#include "fill_ilhs.h"
#include "../../data_structures/node.h"
#include "../../data_structures/edge.h"
#include "../../data_structures/triangle.h"

void mpiPerformMoM(std::map<std::string, std::string> &const_map,
                   std::vector<Triangle> &triangles,
                   std::vector<Edge> &edges,
                   std::vector<Node<double>> &nodes,
                   std::complex<double> *ilhs);
#endif