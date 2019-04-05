#ifndef EXCITATION_H
#define EXCITATION_H

#include <vector>

struct Excitation
{
	int type;
	int label;
	float emag;
	float theta;
	float phi;
	std::vector<int> ports;
};

#endif