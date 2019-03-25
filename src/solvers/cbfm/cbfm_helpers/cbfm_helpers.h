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

	std::vector<std::vector<std::complex<double>*>> z_red; // Reduced Z matrices
	std::complex<double> *z_red_concat; // Concatenated reduced Z matrices
};

struct CBFMVectors
{
	std::vector<std::complex<double>*> v_self; // Normal Vm split by domain
	std::vector<std::complex<double>*> j_prim; // Primary CBF's -> 1 per domain
	std::vector<std::vector<std::complex<double>*>> j_sec; // Secondary CBF's -> multiple per domain
	std::vector<std::complex<double>*> j_cbfm; // Concatenation of j_prim and j_sec per domain
	std::vector<std::complex<double>*> v_red; // Reduced v vectors
	std::complex<double> *v_red_concat; // Concatenated reduced V vectors
};

struct SizeMap
{
    int n_cbfs = 0;
};

void resizeCBFMZMatricesForEqualDomains(CBFMZMatrices &z_matrices, int num_domains, int num_edges_per_domain);
void resizeCBFMVectorsForEqualDomains(CBFMVectors &v_vectors, int num_domains, int num_edges_per_domain);
void resizeCBFMZMatricesForEDD(CBFMZMatrices &z, int num_domains, int domain_size);
void resizeCBFMVAndCBFsForEDD(CBFMVectors &v, int num_domains, int domain_size);
void resizeCBFMZredForEDD(CBFMZMatrices &z,
                          CBFMVectors &v,
                          std::vector<SizeMap> sizes,
                          int domain_size);
void resizeCBFMVredForEDD();

#endif
