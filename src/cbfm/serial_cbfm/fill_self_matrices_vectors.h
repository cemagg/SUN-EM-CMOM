#ifndef FILL_SELF_MATRICES_VECTORS_H
#define FILL_SELF_MATRICES_VECTORS_H

#include <map>
#include <vector>
#include <complex>
#include <algorithm>

#include "../cbfm_helpers/cbfm_helpers.h"

#include "../../data_structures/node.h"
#include "../../data_structures/edge.h"
#include "../../data_structures/triangle.h"
#include "../../data_structures/label.h"

#include "../../mom/serial_mom/fill_vrhs.h"
#include "../../mom/serial_mom/fill_zmn.h"

void fillZVSelfForEDD();

#endif
