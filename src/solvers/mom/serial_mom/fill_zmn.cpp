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
                   Label &source_label,
                   Label &observ_label,
                   bool cbfm)
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
    for(int p = 0; p < observ_label.triangle_indices.size(); p++)
    {
        for(int q = 0; q < source_label.triangle_indices.size(); q++)
        { 
            // Calculate the interactions between a source and observation triangle
            std::vector<Node<std::complex<double>>> a_and_phi = calculateAAndPhi(observ_label.triangle_indices[p],
                                                                                 source_label.triangle_indices[q],
                                                                                 triangles,
                                                                                 nodes,
                                                                                 quad_weights_values,
                                                                                 wave_number,
                                                                                 mu,
                                                                                 omega,
                                                                                 epsilon);

            // Loop over the edges of the source and observation triangles
            for(int e = 0; e < triangles[source_label.triangle_indices[q]].edge_indices.size(); e++)
            {
                for(int r = 0; r < triangles[observ_label.triangle_indices[p]].edge_indices.size(); r++)
                {
                    // Calculate and add the edge contributions to Zmn
                    if(!cbfm)
                    {
                        z_mn[(triangles[observ_label.triangle_indices[p]].edge_indices[r]) + 
                            (triangles[source_label.triangle_indices[q]].edge_indices[e])
                            * source_label.edge_indices.size()] +=
                            delta_zmn(observ_label.triangle_indices[p],
                                   source_label.triangle_indices[q],
                                   edges, triangles, r, e, a_and_phi,omega);
                    }
                    else
                    {
                        z_mn[ (distance(observ_label.edge_indices.begin(),
                                       std::find(observ_label.edge_indices.begin(),
                                       observ_label.edge_indices.end(),
                                       triangles[observ_label.triangle_indices[p]].edge_indices[r]))) + 
                              (distance(source_label.edge_indices.begin(),
                                       std::find(source_label.edge_indices.begin(),
                                       source_label.edge_indices.end(),
                                       triangles[source_label.triangle_indices[q]].edge_indices[e])))
                            * source_label.edge_indices.size()] +=
                            delta_zmn(observ_label.triangle_indices[p],
                                   source_label.triangle_indices[q],
                                   edges, triangles, r, e, a_and_phi,omega);
                    }
                }
            }
        }
    }
}

