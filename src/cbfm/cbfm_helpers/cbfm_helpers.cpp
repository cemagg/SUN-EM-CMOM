#include "cbfm_helpers.h"

void resizeCBFMZMatricesForEqualDomains(CBFMZMatrices &z_matrices, int num_domains, int num_edges_per_domain)
{
	// Allocate memory for z_self, z_self_inv, and z_self_piv
	z_matrices.z_self = new std::complex<double>[num_edges_per_domain * num_edges_per_domain]();
	z_matrices.z_self_inv = new std::complex<double>[num_edges_per_domain * num_edges_per_domain]();
	z_matrices.z_self_piv = new int[num_edges_per_domain]();

	// Allocate for z_couple, z_couple_inv and z_couple_piv

	// First resize the main vectors
	z_matrices.z_couple.resize(num_domains);
	z_matrices.z_red.resize(num_domains);

	// Resize the matrix to store the concatenated reduced Z matrix
	z_matrices.z_red_concat = new std::complex<double>[num_domains * num_domains * num_domains * num_domains];

	// now resize the secondary vectors and allocate to the pointers
	for(int i = 0; i < num_domains; i++)
	{
		z_matrices.z_couple[i].resize(num_domains - 1);
		z_matrices.z_red[i].resize(num_domains);

		for(int k = 0; k < num_domains; k++)
		{
			z_matrices.z_red[i][k] = new std::complex<double>[num_domains * num_domains]();
		}
		
		for(int j = 0; j < (num_domains - 1); j++)
		{
			z_matrices.z_couple[i][j] = new std::complex<double>[num_edges_per_domain * num_edges_per_domain]();
		}
	}	
}

void resizeCBFMVectorsForEqualDomains(CBFMVectors &v_vectors, int num_domains, int num_edges_per_domain)
{
	// Resize the v_self, j_prim and j_sec(main) vectors and allocate memory 
	v_vectors.v_self.resize(num_domains);
	v_vectors.j_prim.resize(num_domains);
	v_vectors.j_sec.resize(num_domains);
    v_vectors.j_cbfm.resize(num_domains);
    v_vectors.v_red.resize(num_domains);

    v_vectors.v_red_concat = new std::complex<double>[num_domains * num_domains];

	for(int i = 0; i < num_domains; i++)
	{
		v_vectors.v_self[i] = new std::complex<double>[num_edges_per_domain]();	
		v_vectors.j_prim[i] = new std::complex<double>[num_edges_per_domain]();
        v_vectors.j_cbfm[i] = new std::complex<double>[num_domains * num_edges_per_domain]();
        v_vectors.v_red[i] = new std::complex<double>[num_domains];	

		v_vectors.j_sec[i].resize(num_domains - 1);
    
    	for(int j = 0; j < (num_domains - 1); j++)
		{
			v_vectors.j_sec[i][j] = new std::complex<double>[num_edges_per_domain]();
		}
	}
}