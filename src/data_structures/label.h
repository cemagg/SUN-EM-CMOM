#ifndef LABEL_H
#define LABEL_H

#include <vector>

struct Label
{
	std::vector<int> triangle_indices;
	std::vector<int> edge_indices;
};

#endif