#include "Mesh.h"

Mesh::Mesh() {
	pos = { 0.0, 0.0, 0.0 };
	scale = 1.0;
	angle = { 0.0, 0.0, 0.0 };

	vertices.clear();
}

void Mesh::AddVertex(float3 vertex) {
	vertices.push_back(vertex);
}

void Mesh::AddVertex(float x, float y, float z) {
	AddVertex({ x, y, z });
}

void Mesh::ReleaseMesh() {
}

