/** 
 * \file edge.h
 * Defining the data structure for a MoM triangle edge
 * 
 * \author Tameez Ebrahim
 * \date 04 March 2019
 */

#ifndef EDGE_H
#define EDGE_H

#include "node.h"


/**
 * @brief      A struct to store all information about an edge
 */
struct Edge
{
    int vertex1;                // Index to node list
    int vertex2;                // Index to node list
    Node<double> centre;        // Centre of the edge
    double length;              // Length of the edge
    int minus_triangle_index;   // Index to negative triangle assosciated with the edge
    int plus_triangle_index;    // Index to positive triangle assosciated with the edge
    int minus_free_vertex;      // Index to negative free vertex of assosciated triangle
    int plus_free_vertex;       // Index to positive free vertex of assosciated triangle
    Node<double> rho_c_minus;   // Rho C Minus
    Node<double> rho_c_plus;    // Rho C Plus
};


#endif // EDGE_H





