#include "dgfm_weights.h"

void calculateDGFMWeights(DGFMRow &row, 
						  DGFMExcitations &excitations,
						  int num_domains,
						  int excitation_index,
						  int domain_index)
{
	// For now just allocate 1+0i for all weights for expediency (sp?)

	int index = 0;

	for (int i = 0; i < num_domains; i++)
	{
		if (i != domain_index)
		{
			row.dgfm_weights[index] = std::complex<double>(1.0, 0.0);
			index++;
		}
	}	
}

