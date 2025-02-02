#include "Mesh.h"

Mesh::Mesh() {
	vertices.clear();
}

void Mesh::AddVertex(point3 vertex) {
	vertices.push_back(vertex);
}

void Mesh::AddVertex(float x, float y, float z) {
	AddVertex({ x, y, z });
}

void Mesh::Scale(float factor) {
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].x *= factor;
		vertices[i].y *= factor;
		vertices[i].z *= factor;
	}
}

void Mesh::ReleaseMesh() {
	vertices.clear();
}

