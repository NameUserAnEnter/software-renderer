#ifndef MESHELEMENTS_H
#define MESHELEMENTS_H

#include "DimensionalTypes.h"

struct Vertex {
	float3 pos;

	void operator=(float3 f) {
		pos = f;
	}
};

#endif

