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
		z_matrices.z_couple[i].resize(num_domains);
		z_matrices.z_red[i].resize(num_domains);

		for(int j = 0; j < num_domains; j++)
		{
			z_matrices.z_red[i][j] = new std::complex<double>[num_domains * num_domains]();
			
			if(j != i)
			{
				z_matrices.z_couple[i][j] = new std::complex<double>[num_edges_per_domain * num_edges_per_domain]();
			}
			else
			{
				z_matrices.z_couple[i][j] = NULL;
			}
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

		v_vectors.j_sec[i].resize(num_domains);
    
    	for(int j = 0; j < num_domains; j++)
		{	
			if(j != i)
			{
				v_vectors.j_sec[i][j] = new std::complex<double>[num_edges_per_domain]();
			}
			else
			{
				v_vectors.j_sec[i][j] = NULL;
			}
		}
	}
}

// This function allocates memory for Zself and Zcouple
void resizeCBFMZMatricesForEDD(CBFMZMatrices &z, int num_domains, int domain_size)
{
    z.z_self = new std::complex<double>[domain_size * domain_size]();
    z.z_self_inv = new std::complex<double>[domain_size * domain_size];
    z.z_self_piv = new int[domain_size];

    z.z_couple.resize(num_domains);
    
    for (int i = 0; i < num_domains; i++)
    {
        z.z_couple[i].resize(num_domains);

        for (int j = 0; j < num_domains; j++)
        {
            if (j != i)
            {
                z.z_couple[i][j] = new std::complex<double>[domain_size *
                                                            domain_size]();
            }
            else
            {
                z.z_couple[i][j] = NULL;
            }
        }
    }
}



// This function allocates for Vself and the BF's
void resizeCBFMVAndCBFsForEDD(CBFMVectors &v, int num_domains, int domain_size)
{
    v.v_self.resize(num_domains);
    v.j_prim.resize(num_domains);
    v.j_sec.resize(num_domains);
    v.j_cbfm.resize(num_domains);

    for (int i = 0; i < num_domains; i++)
    {
        v.v_self[i] = new std::complex<double>[domain_size]();
        v.j_prim[i] = new std::complex<double>[domain_size]();
        v.j_cbfm[i] = new std::complex<double>[domain_size * num_domains]();

        v.j_sec[i].resize(num_domains);

        for (int j = 0; j < num_domains; j++)
        {
            if (j != i)
            {
                v.j_sec[i][j] = new std::complex<double>[domain_size];
            }
            else
            {
                v.j_sec[i][j] = NULL;
            }
        }
    }
}



void resizeCBFMZredForEDD(CBFMZMatrices &z,
                          CBFMVectors &v,
                          std::vector<SizeMap> sizes,
                          int domain_size)
{
    int z_sum = 0;
    int v_sum = 0;
    int z_red_size;
    
    z.z_red.resize(sizes.size());
    v.v_red.resize(sizes.size());
    for (int i = 0; i < sizes.size(); i++)
    {
        z.z_red[i].resize(sizes.size());
        
        v.v_red[i] = new std::complex<double>[sizes[i].n_cbfs];
        v_sum += sizes[i].n_cbfs;
        
        for (int j = 0; j < sizes.size(); j++)
        {
            z_red_size = sizes[i].n_cbfs * sizes[j].n_cbfs;
            z.z_red[i][j] = new std::complex<double>[z_red_size]();
            z_sum += z_red_size;
        }
    }
    z.z_red_concat = new std::complex<double>[z_sum];
    v.v_red_concat = new std::complex<double>[v_sum];
}



void resizeCBFMVredForEDD()
{
    
}
