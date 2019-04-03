#ifndef SERIAL_EDD_CBFM_H
#define SERIAL_EDD_CBFM_H

#include <iostream>

#include "../../../data_structures/node.h"
#include "../../../data_structures/edge.h"
#include "../../../data_structures/triangle.h"
#include "../../../data_structures/label.h"

#include "../../mom/serial_mom/fill_zmn.h"
#include "../../mom/serial_mom/fill_vrhs.h"

#include "../cbfm_helpers/cbfm_helpers.h"

#include "fill_self_matrices_vectors.h"
#include "serial_generate_cbfs.h"
#include "serial_cbf_reducer.h"
#include "serial_z_v_reducer.h"
#include "serial_cbfm_solve_ilhs.h"


void serialEDDCBFM(std::map<std::string, std::string> &const_map,
                   std::map<int, Label> &label_map,
                   std::vector<Triangle> &triangles,
                   std::vector<Edge> &edges,
                   std::vector<Node<double>> &nodes,
                   std::complex<double> *ilhs);

#endif
