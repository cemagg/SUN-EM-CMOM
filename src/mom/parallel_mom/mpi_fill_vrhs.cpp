#include "mpi_fill_vrhs.h"

void mpi_fill_vrhs(std::complex<double> *vrhs,
                   int &total_proc_rows,
                   int &total_proc_cols,
                   int &proc_row,
                   int &proc_col,
                   int &block_size,
                   std::map<std::string, std::string> &const_map,
                   std::vector<Triangle> &triangles,
                   std::vector<Edge> &edges)
                   
{

     int zero = 0;
    int one = 1;

    // Read in details of the plane wave from const_map
    double theta = std::stod(const_map["theta_0"]) * std::stod(const_map["DEG2RAD"]);
    double phi = std::stod(const_map["phi_0"]) * std::stod(const_map["DEG2RAD"]);
    double efield_magnitude = std::stod(const_map["EMag"]);
    int propagation_direction = std::stoi(const_map["prop_direction"]);
    double wavenumber = 2 * M_PI / (std::stod(const_map["C0"]) / std::stod(const_map["cppFreq"]));
    
    IncidentPlaneWave plane_wave = getIncidentPlaneWave(theta,
                                                        phi,
                                                        efield_magnitude,
                                                        propagation_direction,
                                                        wavenumber);

    int index;
    for(int i = 1; i < edges.size() + 1; i++)
    {
        index = i;
        if(indxg2p_(&index, &block_size, &zero, &zero, &total_proc_rows) == proc_row &&
           indxg2p_(&one, &block_size, &zero, &zero, &total_proc_cols) == proc_col)
        {

            vrhs[(indxg2l_(&index, &block_size, &zero, &zero, &total_proc_rows) - 1)] +=
                 getVrhsValueForIncidentPlaneWave((i-1), plane_wave, triangles, edges);
        } 
    }
}

















                                                         