#include "fill_zmn.h"

/**
 *  File:       fill_zmn.cpp
 *  Author:     Tameez Ebrahim
 *  Created:    05 February 2019
 *    
 *  Fill in Zmn for a serial MoM solver 
 *
 */

void serialFillZmn(std::complex<double> *z_mn,
                   std::vector<Edge> &edges,
                   std::vector<Triangle> &triangles,
                   std::vector<Node<double>> &nodes,
                   std::map<std::string, std::string> &const_map,
                   int source_triangle_start,
                   int source_triangle_size,
                   int observ_triangle_start,
                   int observ_triangle_size)
{

    // Get the quadrature weights and values
    std::vector<std::array<double, 4>> quad_weights_values = getQuadratureWeightsAndValues(6);

    // Get the necessary constants from const_map
    double c = C_0;
    double frequency = std::stod(const_map["cppFreq"]);
    double mu = MU_0;
    double epsilon = EPS_0;

    // Calculate secondary values based on the constants
    double omega = 2 * M_PI * frequency;
    double lambda = c / frequency;
    double wave_number = 2 * M_PI / lambda;

    // Loop over the source and observation triangles
    for(int p = observ_triangle_start; p < (observ_triangle_size + observ_triangle_start); p++)
    {
        for(int q = source_triangle_start; q < (source_triangle_size + source_triangle_start); q++)
        {
            // Calculate the interactions between a source and observation triangle
            std::vector<Node<std::complex<double>>> a_and_phi = calculateAAndPhi(p,
                                                                                 q,
                                                                                 triangles,
                                                                                 nodes,
                                                                                 quad_weights_values,
                                                                                 wave_number,
                                                                                 mu,
                                                                                 omega,
                                                                                 epsilon);

            // Loop over the edges of the source and observation triangles
            for(int e = 0; e < triangles[q].edge_indices.size(); e++)
            {
                for(int r = 0; r < triangles[p].edge_indices.size(); r++)
                {
                    // Calculate and add the edge contributions to Zmn
                    z_mn[(triangles[p].edge_indices[r]-observ_triangle_start) + 
                        (triangles[q].edge_indices[e]-source_triangle_start)
                         * source_triangle_size] +=
                         delta_zmn(p, q, edges, triangles, r, e, a_and_phi,omega);

                }
            }
        }
    }
}
