#ifndef MPI_IDGFM_H
#define MPI_IDGFM_H

#include <complex>
#include <vector>
#include <map>
#include <iostream>

#include <mpi.h>

#include "../../../data_structures/node.h"
#include "../../../data_structures/edge.h"
#include "../../../data_structures/triangle.h"
#include "../../../data_structures/label.h"

#include "../dgfm_helpers/dgfm_helpers.h"

void mpiPerformIDGFM(std::map<std::string, std::string> &const_map,
         	        std::map<int, Label> &label_map,
            	     std::vector<Triangle> &triangles,
                	 std::vector<Edge> &edges,
                 	std::vector<Node<double>> &nodes,
                 	std::vector<Excitation> &excitations,
                 	std::complex<double> *ilhs);

#endif