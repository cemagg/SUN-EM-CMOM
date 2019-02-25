#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>

#include "node.h"

struct Triangle
{
    int vertex_1;
    int vertex_2;
    int vertex_3;
    Node<double> centre;
    double area;
    std::vector<int> edge_indices;
    int label;
};

#endif // TRIANGLE_H
