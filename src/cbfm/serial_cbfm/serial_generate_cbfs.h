#ifndef SERIAL_GENERATE_CBFS_H
#define SERIAL_GENERATE_CBFS_H

#include <complex>
#include <vector>

#include "../../data_structures/edge.h""
#include "../../data_structures/label.h""
#include "../../data_structures/triangle.h""
#include "../../data_structures/node.h"

#include "../../mom/serial_mom/fill_zmn.h"

#include "../cbfm_helpers/cbfm_helpers.h"

extern "C"
{
    void zgetrf_(int*, int*, std::complex<double>*, int*, int*, int*);

    void zgetrs_(const char*, int*, int*, std::complex<double>*, int*, int*,
                 std::complex<double>*, int*, int*);

    void zgemv_(const char*, int*, int*, std::complex<double>*, std::complex<double>*,
                int*, std::complex<double>*, int*, std::complex<double>*,
                std::complex<double>*, int*);
}

void generateEDDCBFs(CBFMZMatrices &z,
                     CBFMVectors &v,
                     int num_domains,
                     int domain_size,
                     std::map<std::string, std::string> &const_map,
                     std::map<int, Label> &label_map,
                     std::vector<Triangle> &triangles,
                     std::vector<Edge> &edges,
                     std::vector<Node<double>> &nodes);


#endif
