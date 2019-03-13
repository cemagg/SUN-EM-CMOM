#include <complex>
#include <cmath>
#include <iostream>
#include <chrono>

// https://stackoverflow.com/questions/15092504/how-to-time-a-function-in-milliseconds-without-boosttimer
using Clock = std::chrono::steady_clock;
using std::chrono::time_point;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using namespace std::literals::chrono_literals;

double twoNormError( std::complex<double> *sol,
					 std::complex<double> *ref,
					 int size)
{
	double diff_sum = 0.0;
	double reff_sum = 0.0;

	for (int i = 0; i < size; i++)
	{
		diff_sum += std::pow(std::abs(sol[i] - ref[i]), 2);
		reff_sum += std::pow(std::abs(ref[i]), 2); 
	}

	return (std::sqrt(diff_sum) / std::sqrt(reff_sum)) * 100;
}

void matrixXVector( std::complex<double> *mat, 
					std::complex<double> *vec,
					std::complex<double> *ans,
					int size )
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			ans[i] += mat[j + i * size] * vec[j];
		}
	}
}


int main()
{
	// simple test example for matrix vector product
	// LAPACK to be added later for build simplicity
	// Typical CBFM matrix vector product -> Zself * Jprim_cbf
	// Results to be checked with values from numpy
	 
	int size = 4;
	
	// Zself
	std::complex<double> *z_self = new std::complex<double>[size * size];
	z_self[0] 	= std::complex<double>(-9.83324088e-03,	 1.65688873);
	z_self[1] 	= std::complex<double>( 1.09074423e-04,  0.67106286);
	z_self[2] 	= std::complex<double>( 1.01154065e-04,  0.66491597);
	z_self[3] 	= std::complex<double>( 8.48616235e-03, -0.29883821);
	z_self[4]	= std::complex<double>( 1.09074423e-04,  0.67106286);
	z_self[5] 	= std::complex<double>(-9.83324088e-03,  1.65688873);
	z_self[6] 	= std::complex<double>( 8.48616235e-03, -0.29883821);
	z_self[7] 	= std::complex<double>( 1.01154065e-04,  0.66491597);
	z_self[8] 	= std::complex<double>( 1.05554544e-04,  0.65606459);
	z_self[9] 	= std::complex<double>( 8.48640319e-03, -0.30706501);
	z_self[10] 	= std::complex<double>(-7.70970443e-03,  1.42554161);
	z_self[11] 	= std::complex<double>( 9.73963272e-05,  0.66411422);
	z_self[12] 	= std::complex<double>( 8.48640319e-03, -0.30706501);
	z_self[13] 	= std::complex<double>( 1.05554544e-04,  0.65606459);
	z_self[14] 	= std::complex<double>( 9.73963272e-05,  0.66411422);
	z_self[15] 	= std::complex<double>(-7.70970443e-03,  1.42554161);

	// Jprim_cbf 
	std::complex<double> *j_prim = new std::complex<double>[size];
	j_prim[0] = std::complex<double>(-1.18893844e-05, -0.00129126);	
	j_prim[1] = std::complex<double>( 1.18893844e-05,  0.00129126);	
	j_prim[2] = std::complex<double>(-1.22838444e-05, -0.00127863);	
	j_prim[3] = std::complex<double>( 1.22838444e-05,  0.00127863);	

	// numpy answer
	std::complex<double> *numpy = new std::complex<double>[size];
	numpy[0] = std::complex<double>( 0.00250547, 3.72694497e-20);
	numpy[1] = std::complex<double>(-0.00250547, 2.71050543e-20);
	numpy[2] = std::complex<double>( 0.00221743, 6.77626358e-21);
	numpy[3] = std::complex<double>(-0.00221743,-3.38813179e-21);

	// C++ no library matrix vector product
	// First check error
	std::complex<double> *cpp_ans = new std::complex<double>[size]();

	matrixXVector(z_self, j_prim, cpp_ans, size);
	std::cout << "The c++ error is: " << twoNormError(cpp_ans, numpy, size) << std::endl;

	time_point<Clock> cpp_start = Clock::now();
	for (int i = 0; i < 10000; i++)
	{
		matrixXVector(z_self, j_prim, cpp_ans, size);
	}	
	time_point<Clock> cpp_end = Clock::now();
	milliseconds cpp_time = duration_cast<milliseconds>(cpp_end - cpp_start);
	std::cout << "C++ 10000 iter done in: " << cpp_time.count() << " ms" << std::endl;

}