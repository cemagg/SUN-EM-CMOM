#include "mom.h"

void mpiPerformMoM(std::map<std::string, std::string> &const_map,
				   std::map<int, Label> &label_map,
                   std::vector<Triangle> &triangles,
                   std::vector<Edge> &edges,
                   std::vector<Node<double>> &nodes,
                   std::complex<double> *ilhs)
{
	serialFillVrhs(const_map, triangles, edges, ilhs, label_map[0]);

	std::complex<double> *zmn = new std::complex<double>[edges.size() * edges.size()]();
	serialFillZmn(zmn, edges, triangles, nodes, const_map, label_map[0], label_map[0], false);

	serialFillIlhs(zmn, ilhs, edges.size());	
}