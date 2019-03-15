#include "serial_edd_cbfm.h"

// This function calculates the CBFM for identical non-connected elements
void serialSameDisconnectedCBFM()
{
    // init
    int num_domains;
    int domain_size;
    
    // Declare Structs
    CBFMZMatrices z_mat;
    CBFMVectors v_vec;

    resizeCBFMZMatricesForEDD(z_mat, num_domains, domain_size);
    resizeCBFMVAndCBFsForEDD(v_vec, num_domains, domain_size);
    
    // Fill Zself

    // Fill Vself

    // Calculate CBFprim CBFsec and Zcouple

    // Do SVD if required, else skip

    // Calculare Vred and Zred

    // Calculate Icbfm

    // Map CBF's back to Irwg

}
