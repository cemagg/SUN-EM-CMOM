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
                   std::vector<Node<double>> nodes,
                   std::map<std::string, std::string> &const_map)
{


    // Get the quadrature weights and values
    std::vector<std::array<double, 4>> quad_weights_values = getQuadratureWeightsAndValues(6);

    // Get the necessary constants from const_map
    double c = std::stod(const_map["C0"]);
    double frequency = std::stod(const_map["cppFreq"]);
    double mu = std::stod(const_map["MU_0"]);
    double epsilon = std::stod(const_map["EPS_0"]);

    // Calculate secondary values based on the constants
    double omega = 2 * M_PI * frequency;
    double lambda = c / frequency;
    double wave_number = 2 * M_PI / lambda;

    // Loop over the source and observation triangles
    for(int p = 0; p < triangles.size(); p++)
    {
        for(int q = 0; q < triangles.size(); q++)
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
                    z_mn[triangles[p].edge_indices[r] + triangles[q].edge_indices[e] * edges.size()] +=
                         delta_zmn(p, q, edges, triangles, r, e, a_and_phi,omega);
                }
            }
        }
    }
}
