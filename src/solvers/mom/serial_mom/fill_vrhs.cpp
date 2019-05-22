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
                    int domain_index,
                    Label label,
                    bool cbfm)
{

    // std::vector<std::complex<double>> vrhs;
    // vrhs.resize(edges.size()); 
    
    if(excitations[domain_index].type == 2)
    {
        int port_index = 0;
        std::vector<int> relative_ports;

        relative_ports.resize(excitations[domain_index].ports.size()); 


        for (int i = 0; i < relative_ports.size(); i++)
        {
            if (cbfm)
            {
                relative_ports[i] = distance(label.edge_indices.begin(),
                                            std::find(label.edge_indices.begin(),
                                            label.edge_indices.end(),
                                            std::abs(excitations[domain_index].ports[i]))); 
            }
            else
            {
                relative_ports[i] = excitations[domain_index].ports[i];
            }
        }
        
        for (int i = 0; i < label.edge_indices.size(); i++)
        {

            if (i == std::abs(relative_ports[port_index]))
            {
                if (excitations[domain_index].ports[port_index] < 0)
                {
                    vrhs[i] = getVrhsValueForDeltaGap(edges[std::abs(excitations[domain_index].ports[port_index])].length,
                                                      excitations[domain_index].emag);
                }
                else
                {
                    vrhs[i] =   std::complex<double>(-1.0, 0.0) * 
                                getVrhsValueForDeltaGap(edges[std::abs(excitations[domain_index].ports[port_index])].length,
                                                        excitations[domain_index].emag);
                }
                port_index++;
            }
            else
            {
                vrhs[i] = std::complex<double>(0.0,0.0);    
            }
        }
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
