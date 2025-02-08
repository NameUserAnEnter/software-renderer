#ifndef MESH_H
#define MESH_H

#include "MeshElements.h"
#include "Transformation.h"
#include "Geometry.h"
#include <vector>

class Mesh {
public:
	Mesh();
	void Release();

	// To do: Decide on a mesh representation or implement more than one

	void AddVertex(Vertex);
	std::vector<Vertex> Vertices();

	void ApplyTransformation();

public:
	Transformation t;

private:
	std::vector<Vertex> vertices;
};

#endif

