#ifndef QUADRATURE_H
#define QUADRATURE_H

#include <vector>
#include <array>
#include <cmath>
#include <iostream>

std::vector<std::array<double, 4>> getQuadratureWeightsAndValues(int num_quadrature_points);
std::vector<std::array<double, 2>> getGaussLegendreQuadratureWeightsAndValues(int num_quadrature_points);

#endif // QUADRATURE_H
