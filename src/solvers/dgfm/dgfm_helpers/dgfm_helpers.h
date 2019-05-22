#ifndef DGFM_HELPERS_H
#define DGFM_HELPERS_H

#include <complex>
#include <vector>

struct DGFMRow
{
	std::vector<std::complex<double>*> z_matrices;
	std::complex<double> *dgfm_weights;	
};

struct DGFMExcitations
{
	std::vector<std::complex<double>*> excitations;
};

void allocateDGFMRowMemory(DGFMRow &row, int num_domains, int domain_size);
void allocateDGFMExcitations(DGFMExcitations &dgfm_excitations, int num_domains, int domain_size);

#endif
