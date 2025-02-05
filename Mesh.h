#ifndef MESH_H
#define MESH_H

#include "MeshElements.h"
#include "Transformation.h"
#include <vector>

class Mesh {
public:
	Mesh();
	void Release();

	void AddVertex(Vertex);
	std::vector<Vertex> Vertices();

	void ApplyTransformation();

private:
	std::vector<Vertex> vertices;

public:
	Transformation t;
};

#endif

