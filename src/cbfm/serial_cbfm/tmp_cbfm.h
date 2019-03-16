#ifndef TMP_CBFM_H
#define TMP_CBFM_H

#include <map>
#include <complex>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <fstream>
#include <algorithm>

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

    void zgemm_(const char*, const char*, int*, int*, int*, std::complex<double>*, std::complex<double>*,
    	        int*, std::complex<double>*, int*, std::complex<double>*, std::complex<double>*, int*);

    void zgemv_(const char*, int*, int*, std::complex<double>*, std::complex<double>*, int*,
                std::complex<double>*, int*, std::complex<double>*, std::complex<double>*, int*);

    void zaxpy_(int*, std::complex<double>*, std::complex<double>*, int*, std::complex<double>*, int*);

    void zgesvd_(const char*, const char*, int*, int*, std::complex<double>*, int*, double*,
                 std::complex<double>*, int*, std::complex<double>*, int*, std::complex<double>*,
                 int*, double*, int*);

}

void tperformCBFM(std::map<std::string, std::string> &const_map,
				 std::map<int, Label> &label_map,
                   std::vector<Triangle> &triangles,
                   std::vector<Edge> &edges,
                   std::vector<Node<double>> &nodes,
                   std::complex<double> *ilhs, 
                   bool fpga); 

#endif
