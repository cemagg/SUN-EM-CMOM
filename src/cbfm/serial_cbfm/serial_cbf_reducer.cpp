#include "serial_cbf_reducer.h"

void serialReduceCBFsWithSVD(CBFMVectors &v,
                             std::vector<SizeMap> &sizes,
                             int &red_size,
                             int num_domains,
                             int domain_size)
{
    double threshold = 1e-2;
    
    // LAPACK variables
    char jobu = 'O';
    char jobvt = 'N';
    int info;
    int one = 1;
    int s_length = std::min(num_domains, domain_size);
    int lwork = std::max(1, 2 * s_length +
                         std::max(num_domains, domain_size));

    double *sing_values        = new double[s_length];
    double *rwork              = new double[5 * s_length];
    std::complex<double> *work = new std::complex<double>[lwork];
    
    
    // Create dummy U and V
    std::complex<double> *u;
    std::complex<double> *vt;

    for (int i = 0; i < num_domains; i++)
    {
        zgesvd_(&jobu, &jobvt, &domain_size, &num_domains, v.j_cbfm[i], &domain_size,
                sing_values, u, &one, vt, &one, work, &lwork, rwork, &info);
        // std::cout << "INFO: " << info << std::endl;

        // std::cout <<"I: " <<i << std::endl;
        // for(int m = 0; m < (num_domains * domain_size); m++)
        // {
   
        //     std::cout << v.j_cbfm[i][m] << std::endl;
        // }
        // std::cout << "+++++++++++++++++++" << std::endl;
        

        for (int j = 0; j < s_length; j++)
        {
            if (sing_values[j] > threshold * sing_values[0])
            {
                sizes[i].n_cbfs++;
                red_size++;
            }
            else
            {
                break;
            }
        }
    }

    delete [] sing_values;
    delete [] rwork;
    delete [] work;
}
