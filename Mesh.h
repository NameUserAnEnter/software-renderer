#ifndef MESH_H
#define MESH_H

#include "MeshElements.h"
#include "Transformation.h"
#include "Geometry.h"
#include <vector>

class Mesh {
public:
	Mesh();
	~Mesh();

	void Release();

	void AddVertex(Vertex);
	unsigned int GetVertexCount();

	void ApplyTransformation();

public:
	Vertex* vertices;

	Transformation t;

private:
	unsigned int cVertices;
};

#endif

