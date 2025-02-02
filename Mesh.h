#ifndef MESH_H
#define MESH_H

#include "Point.h"
#include <vector>

class Mesh {
public:
	Mesh();

	void AddVertex(point3);
	void AddVertex(float x, float y, float z);

	void Scale(float);

	void ReleaseMesh();
public:
	std::vector<point3> vertices;
};

#endif

