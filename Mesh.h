#ifndef MESH_H
#define MESH_H

#include "Float.h"
#include <vector>

class Mesh {
public:
	Mesh();

	void AddVertex(float3);
	void AddVertex(float x, float y, float z);
	
	void ReleaseMesh();
public:
	std::vector<float3> vertices;

	float3 pos;
	float scale;
	float3 angle;
};

#endif

