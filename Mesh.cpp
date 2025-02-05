#include "Mesh.h"

Mesh::Mesh() {
	pos = { 0.0, 0.0, 0.0 };
	scale = 1.0;
	angle = { 0.0, 0.0, 0.0 };
}

void Mesh::AddVertex(float3 point) {
	vertices.push_back(point);
}

void Mesh::AddVertex(float x, float y, float z) {
	AddVertex({ x, y, z });
}

std::vector<float3> Mesh::Vertices() {
	return vertices;
}

void Mesh::Release() {
}

