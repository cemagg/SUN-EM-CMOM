#ifndef MPI_FILL_VRHS_H
#define MPI_FILL_VRHS_H

#include <vector>
#include <complex>
#include <map>
#include <string>
#include <iostream>

#include "../mom_by_face/vrhs.h"
#include "../data_structures/triangle.h"
#include "../data_structures/edge.h"

extern "C"
{
	int indxg2l_(int*, int*, int*, int*, int*);
    int indxg2p_(int*, int*, int*, int*, int*);
}

void mpi_fill_vrhs(std::complex<double> *vrhs,
                   int &total_proc_rows,
                   int &total_proc_cols,
                   int &proc_row,
                   int &proc_col,
                   int &block_size,
                   std::map<std::string, std::string> &const_map,
                   std::vector<Triangle> &triangles,
                   std::vector<Edge> &edges);

#endif // MPI_FILL_VRHS_H