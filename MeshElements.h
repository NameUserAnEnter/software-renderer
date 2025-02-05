#ifndef MESHELEMENTS_H
#define MESHELEMENTS_H

#include "Float.h"
#include <vector>

struct Vertex {
	float x, y, z;

	Vertex operator=(float3 f) {
		return { f.x, f.y, f.z };
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

