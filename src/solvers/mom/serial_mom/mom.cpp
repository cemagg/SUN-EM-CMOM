#include "mom.h"

void mpiPerformMoM(std::map<std::string, std::string> &const_map,
				   std::map<int, Label> &label_map,
                   std::vector<Triangle> &triangles,
                   std::vector<Edge> &edges,
                   std::vector<Node<double>> &nodes,
                   std::vector<Excitation> &excitations,
                   std::complex<double> *ilhs)
{
	serialFillVrhs(const_map, triangles, edges, excitations, ilhs, 0, label_map[0], false);

	std::complex<double> *zmn = new std::complex<double>[edges.size() * edges.size()]();
	serialFillZmn(zmn, edges, triangles, nodes, const_map, label_map[0],
                  label_map[0], false);

    // for (int i = 0; i < edges.size(); i++)
    // {
    //     for (int j = 0; j < edges.size(); j++)
    //     {
    //         std::cout << zmn[i + j * edges.size()];
    //     }
    //     std::cout << std::endl;
    // }
	serialFillIlhs(zmn, ilhs, edges.size());	
}

