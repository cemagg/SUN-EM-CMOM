#include "cbfm.h"

void performCBFM(std::map<std::string, std::string> &const_map,
				 std::map<int, std::vector<int>> &label_map,
                   std::vector<Triangle> &triangles,
                   std::vector<Edge> &edges,
                   std::vector<Node<double>> &nodes,
                   std::complex<double> *ilhs)
{
	// Define some constants
	int num_domains = label_map.size(); // number of domains

	//-- Create necessary Zmn Matrices --//
	
	// Create struct to store all z matrices and their inverses
	CBFMZMatrices v_mom_z; // Struct storing the vanilla mom Zmn matrices

	// Lets resize all the struct elements to fit the problem
	resizeCBFMZMatricesForEqualDomains(v_mom_z, num_domains, label_map[0][3]);	

	//-- Create necessary Vm vectors --//
	CBFMVectors v_mom_v;;
		

	//-- Calculate Primary CBF's --//

	//-- Calculate Secondary CBF's --//

	//-- Calculate reduced Zcbfm --//

	//-- Calculate reduced Vcbfm --//

	//-- Calculate reduced Icbfm --//

	//-- Solve Irwg --//
}