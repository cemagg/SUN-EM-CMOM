#ifndef MOM_FILE_READER_H
#define MOM_FILE_READER_H

#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>

#include "../data_structures/node.h"
#include "../data_structures/edge.h"
#include "../data_structures/triangle.h"
#include "../data_structures/label.h"

class MoMFileReader
{
    public:
        MoMFileReader(std::string file_path, bool cbfm);

        std::map<std::string, std::string> getConstMap();
        std::vector<Node<double>> getNodes();
        std::vector<Triangle> getTriangles();
        std::vector<Edge> getEdges();

        std::map<std::string, std::string> const_map;
        std::map<int, Label> label_map;
        std::vector<Node<double>> nodes;
        std::vector<Triangle> triangles;
        std::vector<Edge> edges;

    protected:


        std::vector<std::string> constLineReader(std::string line);
        std::vector<std::string> numberLineReader(std::string line, int num_values);
};

#endif // MOM_FILE_READER_H
