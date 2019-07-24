#include "zmn_by_face.h"

std::vector<std::complex<double>> calculateIpqWithoutSingularity(int &observation_triangle_index,
                                                                 int &source_triangle_index,
                                                                 std::vector<Triangle> &triangles,
                                                                 std::vector<Node<double>> nodes,
                                                                 std::vector<std::array<double, 4>> &quad_weights_values,
                                                                 double &wave_number)
{
    std::vector<std::complex<double>> i_vector;
    i_vector.resize(4);

    for(int i = 0; i < quad_weights_values.size(); i++)
    {
        std::vector<Node<double>> simplex_coords(3);
        simplex_coords[0] = scalarMultiplication(nodes[triangles[source_triangle_index].vertex_1], quad_weights_values[i][1]);
        simplex_coords[1] = scalarMultiplication(nodes[triangles[source_triangle_index].vertex_2], quad_weights_values[i][2]);
        simplex_coords[2] = scalarMultiplication(nodes[triangles[source_triangle_index].vertex_3], quad_weights_values[i][3]);

        Node<double> r_prime = addNodes(simplex_coords);
        double R_p = euclideanDistance(triangles[observation_triangle_index].centre, r_prime);

        std::complex<double> greens_function = std::exp(std::complex<double>(-1.0,0) *
                                                        std::complex<double>(0,1) * wave_number * R_p) / R_p;

        i_vector[0] += std::complex<double>(0.5,0) * quad_weights_values[i][0] * greens_function;
        i_vector[1] += std::complex<double>(0.5,0) * quad_weights_values[i][0] * greens_function * quad_weights_values[i][1];
        i_vector[2] += std::complex<double>(0.5,0) * quad_weights_values[i][0] * greens_function * quad_weights_values[i][2];
    }

    i_vector[3] = i_vector[0] - i_vector[1] - i_vector[2];
    return i_vector;
}



std::vector<std::complex<double>> calculateIpq(int &observation_triangle_index,
                                               int &source_triangle_index,
                                               std::vector<Triangle> &triangles,
                                               std::vector<Node<double>> nodes,
                                               std::vector<std::array<double, 4>> &quad_weights_values,
                                               double &wave_number)
{
    bool sing = false;
    if(observation_triangle_index == source_triangle_index && sing == true)
    {
        // TODO: Add sing methods here
        std::vector<std::complex<double>> x;
        return x;
    }
    else
    {
        return calculateIpqWithoutSingularity(observation_triangle_index,
                                              source_triangle_index,
                                              triangles,
                                              nodes,
                                              quad_weights_values,
                                              wave_number);
    }
}

std::vector<Node<std::complex<double>>> calculateAAndPhi(int &observation_triangle_index,
                                                         int &source_triangle_index,
                                                         std::vector<Triangle> &triangles,
                                                         std::vector<Node<double>> nodes,
                                                         std::vector<std::array<double, 4>> &quad_weights_values,
                                                         double &wave_number,
                                                         double &mu,
                                                         double &omega,
                                                         double &epsilon)
{
    std::vector<std::complex<double>> i_vector = calculateIpq(observation_triangle_index,
                                                              source_triangle_index,
                                                              triangles,
                                                              nodes,
                                                              quad_weights_values,
                                                              wave_number);

    std::vector<Node<std::complex<double>>> a_phi_vector(4);


    for(int i = 0; i < 3; i++)
    {
        std::vector<Node<std::complex<double>>> a_pq_nodes(4);
        a_pq_nodes[0] = scalarMultiplication(nodes[triangles[source_triangle_index].vertex_1], i_vector[1]);
        a_pq_nodes[1] = scalarMultiplication(nodes[triangles[source_triangle_index].vertex_2], i_vector[2]);
        a_pq_nodes[2] = scalarMultiplication(nodes[triangles[source_triangle_index].vertex_3], i_vector[3]);

        switch (i) {
            case 0:
                a_pq_nodes[3] = scalarMultiplication(nodes[triangles[source_triangle_index].vertex_1], i_vector[0]);
                break;
            case 1:
                a_pq_nodes[3] = scalarMultiplication(nodes[triangles[source_triangle_index].vertex_2], i_vector[0]);
                break;
            case 2:
                a_pq_nodes[3] = scalarMultiplication(nodes[triangles[source_triangle_index].vertex_3], i_vector[0]);
                break;
        }

        a_pq_nodes[3] = scalarMultiplication(a_pq_nodes[3], -1.0);

        Node<std::complex<double>> sum_a_pq_nodes = addNodes(a_pq_nodes);
        a_phi_vector[i] = scalarMultiplication(sum_a_pq_nodes, (mu / (4 * M_PI)));
    }

    a_phi_vector[3].x_coord = i_vector[0] / (std::complex<double>(0,1) *
                                             std::complex<double>(2,0) *
                                             M_PI *
                                             omega *
                                             epsilon);

    return a_phi_vector;
}

std::complex<double> delta_zmn(int observation_triangle_index, int source_triangle_index,
                               std::vector<Edge> &edges, std::vector<Triangle> &triangles,
                               int observation_triangle_edge, int source_triangle_edge,
                               std::vector<Node<std::complex<double>>> &a_and_phi,
                               double &omega)
{
    Node<std::complex<double>> a_pq;
    //TODO: Format this better
    if(triangles[source_triangle_index].vertex_1 == edges[triangles[source_triangle_index].edge_indices[source_triangle_edge]].plus_free_vertex ||
       triangles[source_triangle_index].vertex_1 == edges[triangles[source_triangle_index].edge_indices[source_triangle_edge]].minus_free_vertex)
    {
        a_pq = scalarMultiplication(a_and_phi[0], edges[triangles[source_triangle_index].edge_indices[source_triangle_edge]].length);
    }
    else if (triangles[source_triangle_index].vertex_2 == edges[triangles[source_triangle_index].edge_indices[source_triangle_edge]].plus_free_vertex ||
            triangles[source_triangle_index].vertex_2 == edges[triangles[source_triangle_index].edge_indices[source_triangle_edge]].minus_free_vertex)
    {
        a_pq = scalarMultiplication(a_and_phi[1], edges[triangles[source_triangle_index].edge_indices[source_triangle_edge]].length);
    }
    else if (triangles[source_triangle_index].vertex_3 == edges[triangles[source_triangle_index].edge_indices[source_triangle_edge]].plus_free_vertex ||
            triangles[source_triangle_index].vertex_3 == edges[triangles[source_triangle_index].edge_indices[source_triangle_edge]].minus_free_vertex)
    {
        a_pq = scalarMultiplication(a_and_phi[2], edges[triangles[source_triangle_index].edge_indices[source_triangle_edge]].length);
    }

    std::complex<double> phi = a_and_phi[3].x_coord * edges[triangles[source_triangle_index].edge_indices[source_triangle_edge]].length;

    if(edges[triangles[source_triangle_index].edge_indices[source_triangle_edge]].minus_triangle_index == source_triangle_index)
    {
        phi *= std::complex<double>(-1.0,0);
    }
    else
    {
        a_pq = scalarMultiplication(a_pq, -1.0);
    }

    Node<double> rho_c;

    if(edges[triangles[observation_triangle_index].edge_indices[observation_triangle_edge]].minus_triangle_index == observation_triangle_index)
    {
        rho_c = edges[triangles[observation_triangle_index].edge_indices[observation_triangle_edge]].rho_c_minus;
        phi *= std::complex<double>(-1.0,0);
    }
    else
    {
        rho_c = edges[triangles[observation_triangle_index].edge_indices[observation_triangle_edge]].rho_c_plus;
    }


    return edges[triangles[observation_triangle_index].edge_indices[observation_triangle_edge]].length *
            (std::complex<double>(0,1) * omega * dotProduct(a_pq, rho_c) / std::complex<double>(2,0) - phi);
}































