#include "mom_file_writer.h"

void writeIlhsToFile(std::complex<double> *ilhs,
					 int matrix_size,
					 std::string file_name)
{
	// Lets write the current vector to a file
	// The form will be a+bi for MATLAB to read easily

	// Lets start by defining a temporary variable to store the imaginary piece(b)
	// and a variable for the sign
	double temp_imag;
	std::string sign;

	// Lets open the file
	std::ofstream file;	
	file.open(file_name.substr(0 , file_name.size() - 3) + "sol");
	file << "NUM_SOLS" << "\t" << 1 << std::endl;
	file << "LENGTH" << "\t" << matrix_size << std::endl;

	for(int i = 0; i < matrix_size; i++)
	{
		temp_imag = ilhs[i].imag();

		// Check if the imaginary part is negative
		// If it is, set sign to - and multiply the imaginary part by -1
		// Else, set sign to +
		if(temp_imag < 0)
		{
			temp_imag *= -1.000;
			sign = "-";
		}
		else
		{
			sign = "+";
		}

		// Lets write a+bi to the file
		// file << ilhs[i].real() << sign << temp_imag << "i" << std::endl;
		file << ilhs[i] << std::endl;
	}	

}

