#ifndef CBFM_HELPERS_H
#define CBFM_HELPERS_H

#include <complex>
#include <vector>
#include <iostream>

struct CBFMZMatrices 
{
	std::complex<double> *z_self; // Zmn self matrix
	std::complex<double> *z_self_inv; // Inverse of Zmn self matrix
	int *z_self_piv; // Pivot vector of inverse Zmn self

	std::vector<std::vector<std::complex<double>*>> z_couple; // Vector of vectors for Zmn coupling matrices

	std::vector<std::vector<std::complex<double>*>> z_red;
};

struct CBFMVectors
{
	std::vector<std::complex<double>*> v_self; // Normal Vm split by domain
	std::vector<std::complex<double>*> j_prim; // Primary CBF's -> 1 per domain
	std::vector<std::vector<std::complex<double>*>> j_sec; // Secondary CBF's -> multiple per domain
	std::vector<std::complex<double>*> j_cbfm; // Concatenation of j_prim and j_sec per domain
};

void resizeCBFMZMatricesForEqualDomains(CBFMZMatrices &z_matrices, int num_domains, int num_edges_per_domain);
void resizeCBFMVectorsForEqualDomains(CBFMVectors &v_vectors, int num_domains, int num_edges_per_domain);

#endif