#ifndef NODE_H
#define NODE_H

#include <complex>
#include <vector>

template<typename T>
struct Node
{
    T x_coord;
    T y_coord;
    T z_coord;
};

template <typename T, typename U>
T dotProduct(Node<T> node1, Node<U> node2)
{
    return (node1.x_coord * node2.x_coord) +
            (node1.y_coord * node2.y_coord) +
            (node1.z_coord * node2.z_coord);
}

template <typename T>
Node<T> scalarMultiplication(Node<double> node, T scalar)
{
    Node<T> return_node;

    return_node.x_coord = node.x_coord * scalar;
    return_node.y_coord = node.y_coord * scalar;
    return_node.z_coord = node.z_coord * scalar;

    return return_node;
}

template  <typename T>
Node<std::complex<double>> scalarMultiplication(Node<std::complex<double>> node, T scalar)
{
    Node<std::complex<double>> return_node;

    return_node.x_coord = node.x_coord * scalar;
    return_node.y_coord = node.y_coord * scalar;
    return_node.z_coord = node.z_coord * scalar;

    return return_node;
}

template <typename T>
Node<T> addNodes(std::vector<Node<T>> nodes)
{
    Node<T> return_node = {0, 0, 0};

    for(int i = 0; i < nodes.size(); i++)
    {
        return_node.x_coord += nodes[i].x_coord;
        return_node.y_coord += nodes[i].y_coord;
        return_node.z_coord += nodes[i].z_coord;
    }

    return return_node;
}

template <typename T>
double norm(Node<T> node)
{
    return std::sqrt(std::pow(node.x_coord, 2) + std::pow(node.y_coord, 2) + std::pow(node.z_coord, 2));
}

template <typename T>
double euclideanDistance(Node<T> node1, Node<T> node2)
{
    Node<T> temp_node;

    temp_node.x_coord = node1.x_coord - node2.x_coord;
    temp_node.y_coord = node1.y_coord - node2.y_coord;
    temp_node.z_coord = node1.z_coord - node2.z_coord;

    return norm(temp_node);
}

#endif // NODE_H
