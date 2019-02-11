#ifndef FILL_VRHS_H
#define FILL_VRHS_H

#include <vector>
#include <complex>
#include <map>
#include <string>

#include "../mom_by_face/vrhs.h"
#include "../data_structures/triangle.h"
#include "../data_structures/edge.h"

std::vector<std::complex<double>> serialFillVrhs(std::map<std::string, std::string> &const_map,
												 std::vector<Triangle> &triangles,
												 std::vector<Edge> &edges);

#endif // FILL_VRHS_H
