#include "cbfm.h"

void performCBFM(std::map<std::string, std::string> &const_map,
                 std::map<int, Label> &label_map,
                 std::vector<Triangle> &triangles,
                 std::vector<Edge> &edges,
                 std::vector<Node<double>> &nodes,
                 std::complex<double> *ilhs)
{
    bool edd_cbfm = true;

    if (edd_cbfm)
    {
        serialEDDCBFM(const_map, label_map, triangles, edges, nodes, ilhs);
    }
    
}
