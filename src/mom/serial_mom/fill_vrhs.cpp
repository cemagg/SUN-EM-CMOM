#include "fill_vrhs.h"

/**
 *  File:       fill_vrhs.cpp
 *  Author:     Tameez Ebrahim
 *  Created:    05 February 2019
 *    
 *  Fill in Vrhs for a serial MoM solver 
 *
 */

std::vector<std::complex<double>> serialFillVrhs(std::map<std::string, std::string> &const_map,
                                                 std::vector<Triangle> &triangles, 
                                                 std::vector<Edge> &edges)
{

    std::vector<std::complex<double>> vrhs;
    vrhs.resize(edges.size());

    // Test whether(sp?) the excitation is an edge feed or plane wave
    if(std::stoi(const_map["edge_feed"]) == 1)
    {
        int delta_gap_index = std::stoi(const_map["feed_edge"]) - 1;
        double efield_magnitude = std::stod(const_map["EMag"]);

        vrhs[delta_gap_index] = getVrhsValueForDeltaGap(edges[delta_gap_index].length, efield_magnitude);
    }
    else
    {
        // Read in details of the plane wave from const_map
        double theta = std::stod(const_map["theta_0"]) * std::stod(const_map["DEG2RAD"]);
        double phi = std::stod(const_map["phi_0"]) * std::stod(const_map["DEG2RAD"]);
        double efield_magnitude = std::stod(const_map["EMag"]);
        int propagation_direction = std::stoi(const_map["prop_direction"]);
        double wavenumber = 2 * M_PI / (std::stod(const_map["C0"]) / std::stod(const_map["cppFreq"]));

        // Create a plane wave in the correct format
        // A conversion from spherical to cartesian is done
        IncidentPlaneWave incident_plane_wave = getIncidentPlaneWave(theta, phi, efield_magnitude, propagation_direction, wavenumber);

        for(int i = 0; i < edges.size(); i++)
        {
            vrhs[i] = getVrhsValueForIncidentPlaneWave(i, incident_plane_wave, triangles, edges);
        }
    }

    return vrhs;
}
