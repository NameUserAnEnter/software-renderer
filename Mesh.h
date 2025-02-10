#ifndef MESH_H
#define MESH_H

#include "MeshElements.h"
#include "Transformation.h"
#include "Geometry.h"
#include <vector>

enum MESH_TOPOLOGY {
	POINT_LIST,
	LINE_LIST,
	LINE_STRIP,
	TRIANGLE_LIST,
	TRIANGLE_STRIP,
	QUAD_LIST
};

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
	Vertex* outputBuffer;		// transformed coordinates can be written to that buffer

	Transformation t;

	MESH_TOPOLOGY topology;

private:
	unsigned int cVertices;
};

#endif

