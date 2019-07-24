/**
 * \file label.h
 * Defining the data structure for labelling MoM triangles and edges 
 * 
 * \author Tameez Ebrahim
 * \date   04 March 2019
 */

#ifndef LABEL_H
#define LABEL_H

#include <vector>

/**
 * @brief      A label that stores assosciated MoM triangle and edge indices
 */
struct Label
{
	std::vector<int> triangle_indices;	// Vector storing assosciated MoM triangle indices
	std::vector<int> edge_indices;		// Vector storing assosciated MoM edge indices
};


#endif



