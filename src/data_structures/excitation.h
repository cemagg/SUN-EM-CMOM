#ifndef EXCITATION_H
#define EXCITATION_H

#include <vector>

struct Excitation
{
	int type;
	int label;
	double emag;
	double theta;
	double phi;
	std::vector<int> ports;
};

#endif
