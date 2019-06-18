#ifndef DGFM_ROW_H
#define DGFM_ROW_H

#include <vector>
#include <complex>
#include <map>
#include <chrono>

#include "../dgfm_helpers/dgfm_helpers.h"
#include "../dgfm_helpers/dgfm_weights.h"

#include "../../mom/serial_mom/fill_vrhs.h"
#include "../../mom/serial_mom/fill_zmn.h"
#include "../../mom/serial_mom/fill_ilhs.h"

void fillDGFMExcitations(DGFMExcitations &v_vectors,
					  	int num_domains,
					  	int domain_size,
					  	std::map<std::string, std::string> &const_map,
                 	  	std::map<int, Label> &label_map,
                 	  	std::vector<Triangle> &triangles,
                 	  	std::vector<Edge> &edges,
                 	  	std::vector<Node<double>> &nodes,
                 	  	std::vector<Excitation> &excitations);

void calculateDGFMRow(DGFMRow &row,
		      DGFMExcitations &v_vectors,
		      int domain_index,
		      int num_domains,
		      int domain_size,
                      bool use_threading,
                      std::map<std::string, std::string> &const_map,
                      std::map<int, Label> &label_map,
                      std::vector<Triangle> &triangles,
                      std::vector<Edge> &edges,
                      std::vector<Node<double>> &nodes,
                      std::vector<Excitation> &excitations);

#endif
