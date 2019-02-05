#ifndef EDGE_H
#define EDGE_H

#include "node.h"

struct Edge
{
    int vertex1;
    int vertex2;
    Node<double> centre;
    double length;
    int minus_triangle_index;
    int plus_triangle_index;
    int minus_free_vertex;
    int plus_free_vertex;
    Node<double> rho_c_minus;
    Node<double> rho_c_plus;
};


#endif // EDGE_H
