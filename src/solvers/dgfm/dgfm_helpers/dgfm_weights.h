#ifndef DGFM_WEIGHTS_H
#define DGFM_WEIGHTS_H

#include <vector>
#include <complex>
#include "dgfm_helpers.h"

// TODO Change index to an excitation struct
void calculateDGFMWeights(DGFMRow &row,
						  DGFMExcitations &excitations,
						  int num_domains, 
						  int excitation_index, 
						  int domain_index);

#endif
