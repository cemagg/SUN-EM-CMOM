#include "dgfm_helpers.h"


void allocateDGFMRowMemory(DGFMRow &row, int num_domains, int domain_size)
{
	// Allocate Z Matrices
	row.z_matrices.resize(num_domains);

	for (int i = 0; i < num_domains; i++)
	{
		row.z_matrices[i] = new std::complex<double>[domain_size * domain_size]; 
	}

	// Allocate Weights
	row.dgfm_weights = new std::complex<double>[num_domains - 1];
}

void allocateDGFMExcitations(DGFMExcitations &dgfm_excitations, int num_domains, int domain_size)
{
	dgfm_excitations.excitations.resize(num_domains);

	for (int i = 0; i < num_domains; i++)
	{
		dgfm_excitations.excitations[i] = new std::complex<double>[domain_size];
	}
}
