#ifndef MPI_FILL_ZMN_H
#define MPI_FILL_ZMN_H

#include <complex>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>

#include "../data_structures/node.h"
#include "../data_structures/edge.h"
#include "../data_structures/triangle.h"
#include "../mom_by_face/quadrature.h"
#include "../mom_by_face/zmn_by_face.h"

extern "C"
{
	int indxg2l_(int*, int*, int*, int*, int*);
    int indxg2p_(int*, int*, int*, int*, int*);
}

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
                std::vector<Node<double>> nodes);

#endif