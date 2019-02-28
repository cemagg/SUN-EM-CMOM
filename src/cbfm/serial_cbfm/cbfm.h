#ifndef CBFM_H
#define CBFM_H

#include <map>
#include <complex>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <fstream>

#include "../../data_structures/node.h"
#include "../../data_structures/edge.h"
#include "../../data_structures/triangle.h"
#include "../../data_structures/label.h"
#include "../../mom/serial_mom/fill_zmn.h"
#include "../../mom/serial_mom/fill_vrhs.h"
#include "../cbfm_helpers/cbfm_helpers.h"

extern "C"
{
    void zgetrf_(int*, int*, std::complex<double>*, int*, int*, int*);
    void zgetrs_(const char*, int*, int*, std::complex<double>*, int*, int*, std::complex<double>*, int*, int*);
}

void performCBFM(std::map<std::string, std::string> &const_map,
				 std::map<int, Label> &label_map,
                   std::vector<Triangle> &triangles,
                   std::vector<Edge> &edges,
                   std::vector<Node<double>> &nodes,
                   std::complex<double> *ilhs); 

#endif