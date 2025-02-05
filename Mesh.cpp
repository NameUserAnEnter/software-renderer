#include "Mesh.h"

Mesh::Mesh() {
}

void Mesh::Release() {
}

void Mesh::AddVertex(Vertex point) {
	vertices.push_back(point);
}

std::vector<Vertex> Mesh::Vertices() {
	return vertices;
}

void Mesh::ApplyTransformation() {
	for (auto& vertex : vertices) {
		vertex.x;
	}
}

