#include "vrhs.h"
#include <iostream>

IncidentPlaneWave getIncidentPlaneWave(double &theta,
                                       double &phi,
                                       double &efield_magnitude,
                                       int &propagation_direction,
                                       double &wavenumber)
{
    IncidentPlaneWave plane_wave;

    if(propagation_direction == 0)
    {
        plane_wave.e_field.x_coord = efield_magnitude * std::cos(phi) * std::cos(theta);
        plane_wave.e_field.y_coord = efield_magnitude * std::sin(phi) * std::cos(theta);

        if(theta == M_PI)
        {
            plane_wave.e_field.z_coord = 0;
        }
        else
        {
            plane_wave.e_field.z_coord = efield_magnitude * -std::sin(theta);
        }
    }
    else if (propagation_direction == 1)
    {
        //TODO: add for phi direction
    }

    plane_wave.propagation_vector.x_coord = wavenumber * std::sin(theta) * std::cos(phi);
    plane_wave.propagation_vector.y_coord = wavenumber * std::sin(theta) * std::sin(phi);
    plane_wave.propagation_vector.z_coord = wavenumber * std::cos(theta);

    return plane_wave;
}

std::complex<double> getVrhsValueForIncidentPlaneWave(int edge_index,
                                                      IncidentPlaneWave &plane_wave,
                                                      std::vector<Triangle> &triangles,
                                                      std::vector<Edge> &edges)
{
    int positive_triangle_index = edges[edge_index].plus_triangle_index;
    int negative_triangle_index = edges[edge_index].minus_triangle_index;

    Node<std::complex<double>> e_plus =
        scalarMultiplication(plane_wave.e_field,
                             std::exp(std::complex<double>(0,1) *
                             dotProduct(plane_wave.propagation_vector,
                             triangles[positive_triangle_index].centre)));
    
    Node<std::complex<double>> e_minus =
        scalarMultiplication(plane_wave.e_field,
                             (std::exp(std::complex<double>(0,1) *
                             dotProduct(plane_wave.propagation_vector,
                             triangles[negative_triangle_index].centre))));

    return (0.5 * dotProduct(e_plus, edges[edge_index].rho_c_plus) +
            0.5 * dotProduct(e_minus, edges[edge_index].rho_c_minus)) *
            edges[edge_index].length;
}

std::complex<double> getVrhsValueForDeltaGap(double &edge_length, double &efield_magnitude)
{
    return edge_length * efield_magnitude;
}

























