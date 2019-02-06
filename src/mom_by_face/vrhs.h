#ifndef VRHS_H
#define VRHS_H

#include <complex>
#include <cmath>
#include <vector>

#include "../data_structures/node.h"
#include "../data_structures/edge.h"
#include "../data_structures/triangle.h"

struct IncidentPlaneWave
{
    Node<double> e_field;
    Node<double> propagation_vector;
};

IncidentPlaneWave getIncidentPlaneWave(double &theta,
									   double &phi,
									   double &efield_magnitude,
									   int    &propagation_direction,
									   double &wavenumber);

std::complex<double> getVrhsValueForIncidentPlaneWave(int edge_index,
													  IncidentPlaneWave &plane_wave,
													  std::vector<Triangle> &triangles,
													  std::vector<Edge> &edges);

std::complex<double> getVrhsValueForDeltaGap(double &edge_length, double &efield_magnitude);

#endif // VRHS_H
