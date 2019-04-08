#include "fill_vrhs.h"

/**
 *  File:       fill_vrhs.cpp
 *  Author:     Tameez Ebrahim
 *  Created:    05 February 2019
 *    
 *  Fill in Vrhs for a serial MoM solver 
 *
 */

void serialFillVrhs(std::map<std::string, std::string> &const_map,
                    std::vector<Triangle> &triangles, 
                    std::vector<Edge> &edges,
                    std::vector<Excitation> &excitations,
                    std::complex<double> *vrhs,
                    Label label)
{

    // std::vector<std::complex<double>> vrhs;
    // vrhs.resize(edges.size()); 
    bool port = false;
    if(port)
    {
        int delta_gap_index = std::stoi(const_map["feed_edge"]) - 1;
        double efield_magnitude = std::stod(const_map["emag"]);

        for (int i = 0; i < label.edge_indices.size(); i++)
        {
            
        }
        vrhs[delta_gap_index] = getVrhsValueForDeltaGap(edges[delta_gap_index].length,
                                                        efield_magnitude);
    }
    else
    {
        // Assume single plane wave
        double theta = excitations[0].theta * DEG2RAD;
        double phi =  excitations[0].phi * DEG2RAD;
        double efield_magnitude = excitations[0].emag;
        int prop_direction = 0;//std::stoi(const_map["prop_direction"]);
        double wavenumber = 2 * M_PI / ((C_0) / std::stod(const_map["cppFreq"]));

        // Create a plane wave in the correct format
        // A conversion from spherical to cartesian is done
        IncidentPlaneWave incident_plane_wave = getIncidentPlaneWave(theta, phi,
                                                                     efield_magnitude,
                                                                     prop_direction,
                                                                     wavenumber);

        for(int i = 0; i < label.edge_indices.size(); i++)
        {
            vrhs[i] = getVrhsValueForIncidentPlaneWave(label.edge_indices[i],
                                                       incident_plane_wave, triangles,
                                                       edges);
        }
    }
}
