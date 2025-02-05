#ifndef MESHELEMENTS_H
#define MESHELEMENTS_H

#include "Float.h"
#include <vector>

struct Vertex {
	float x, y, z;

	void operator=(float3 f) {
		x = f.x;
		y = f.y;
		z = f.z;
	}
};

struct Edge {
	Vertex a;
	Vertex b;
};

struct Face {
	std::vector<Edge> edges;
};

#endif

