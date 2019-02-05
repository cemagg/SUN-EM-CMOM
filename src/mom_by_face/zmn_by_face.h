#ifndef ZMN_BY_FACE_H
#define ZMN_BY_FACE_H

#include <vector>
#include <complex>
#include <cmath>

#include "quadrature.h"
#include "../data_structures/triangle.h"
#include "../data_structures/node.h"
#include "../data_structures/edge.h"

std::vector<std::complex<double>> calculateIpqWithoutSingularity(int &observation_triangle_index,
                                                                 int &source_triangle_index,
                                                                 std::vector<Triangle> &triangles,
                                                                 std::vector<Node<double>> nodes,
                                                                 std::vector<std::array<double, 4>> &quad_weights_values,
                                                                 double &wave_number);

std::vector<std::complex<double>> calculateIpq(int &observation_triangle_index,
                                               int &source_triangle_index,
                                               std::vector<Triangle> &triangles,
                                               std::vector<Node<double>> nodes,
                                               std::vector<std::array<double, 4>> &quad_weights_values,
                                               double &wave_number);

std::vector<Node<std::complex<double>>> calculateAAndPhi(int &observation_triangle_index,
                                                         int &source_triangle_index,
                                                         std::vector<Triangle> &triangles,
                                                         std::vector<Node<double>> nodes,
                                                         std::vector<std::array<double, 4>> &quad_weights_values,
                                                         double &wave_number,
                                                         double &mu,
                                                         double &omega,
                                                         double &epsilon);

std::complex<double> delta_zmn(int observation_triangle_index,
                               int source_triangle_index,
                               std::vector<Edge> &edges,
                               std::vector<Triangle> &triangles,
                               int observation_triangle_edge,
                               int source_triangle_edge,
                               std::vector<Node<std::complex<double>>> &a_and_phi,
                               double &omega);

#endif // ZMN_BY_FACE_H
