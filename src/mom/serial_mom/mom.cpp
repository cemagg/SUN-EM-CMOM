#include "mom.h"

void mpiPerformMoM(std::map<std::string, std::string> &const_map,
                   std::vector<Triangle> &triangles,
                   std::vector<Edge> &edges,
                   std::vector<Node<double>> &nodes,
                   std::complex<double> *ilhs)
{
	serialFillVrhs(const_map, triangles, edges, ilhs);

	std::complex<double> *zmn = new std::complex<double>[edges.size() * edges.size()]();
	serialFillZmn(zmn, edges, triangles, nodes, const_map);

	serialFillIlhs(zmn, ilhs, edges.size());	
}