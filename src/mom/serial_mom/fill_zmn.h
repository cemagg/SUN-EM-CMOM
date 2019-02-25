#ifndef FILL_ZMN_H
#define FILL_ZMN_H

#include <complex>
#include <vector>
#include <map>

#include "../../data_structures/node.h"
#include "../../data_structures/edge.h"
#include "../../data_structures/triangle.h"
#include "../mom_helpers/quadrature.h"
#include "../mom_helpers/zmn_by_face.h"
#include "../../helpers/globals.h"


void serialFillZmn(std::complex<double> *z_mn,
				   std::vector<Edge> &edges,
				   std::vector<Triangle> &triangles,
				   std::vector<Node<double>> nodes,
                   std::map<std::string, std::string> &const_map);

#endif // FILL_ZMN_H
