#include "mpi_fill_zmn.h"

void mpiFillZmn(std::complex<double> *zmn,
                int &num_cols,
                int &total_proc_rows,
                int &total_proc_cols,
                int &proc_row,
                int &proc_col,
                int &block_size,
                std::map<std::string, std::string> &const_map,
                std::vector<Triangle> &triangles,
                std::vector<Edge> &edges,
                std::vector<Node<double>> nodes,
                Label &source_label,
                Label &observ_label)
{
	int zero = 0;
	std::vector<bool> flag(triangles.size());
	std::vector<Node<std::complex<double>>> a_and_phi;

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

	for(int q = 0; q < source_label.triangle_indices.size(); q++)
	{
		std::fill(flag.begin(), flag.end(), false);

		for(int p = 0; p < observ_label.triangle_indices.size(); p++)
		{
			for(int e = 0; e < triangles[source_label.triangle_indices[q]].edge_indices.size(); e++)
			{
				int global_source_edge_index = triangles[source_label.triangle_indices[q]].edge_indices[e] + 1;

				if(indxg2p_(&global_source_edge_index, &block_size, &zero, &zero, &total_proc_rows) == proc_row)
				{
					int local_row_index = indxg2l_(&global_source_edge_index, &block_size, &zero, &zero, &total_proc_rows) - 1;

					for(int r = 0; r < triangles[observ_label.triangle_indices[p]].edge_indices.size(); r++)
					{
						int global_observation_edge_index = triangles[observ_label.triangle_indices[p]].edge_indices[r] + 1;
						if(indxg2p_(&global_observation_edge_index, &block_size, &zero, &zero, &total_proc_cols) == proc_col)
						{
							if(!flag[p])
							{
								a_and_phi = calculateAAndPhi(observ_label.triangle_indices[p],
											         	     source_label.triangle_indices[q],
															 triangles,
															 nodes,
															 quad_weights_values,
															 wave_number,
															 mu,
															 omega,
															 epsilon);
								flag[p] = true;
							}
							int local_col_index = indxg2l_(&global_observation_edge_index, &block_size, &zero, &zero, &total_proc_cols) - 1;

							zmn[local_row_index + local_col_index * num_cols] +=
								delta_zmn(observ_label.triangle_indices[p], 
									source_label.triangle_indices[q], edges, triangles, r, e, a_and_phi, omega);
						}
					}
				}
			}
		}
	}
}

























