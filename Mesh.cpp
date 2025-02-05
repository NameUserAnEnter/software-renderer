#include "Mesh.h"

Mesh::Mesh() {
	t.pos = { 0, 0, 0 };
	t.angle = { 0, 0, 0 };
	t.scale = { 1, 1, 1 };
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
		vertex = Geometry::RotateAroundAxisY({ vertex.x, vertex.y, vertex.z }, t.angle.y);
		vertex = Geometry::RotateAroundAxisX({ vertex.x, vertex.y, vertex.z }, t.angle.x);
		vertex = Geometry::RotateAroundAxisZ({ vertex.x, vertex.y, vertex.z }, t.angle.z);

		vertex = Geometry::Scale({ vertex.x, vertex.y, vertex.z }, t.scale);

		vertex = Geometry::Translate({ vertex.x, vertex.y, vertex.z }, t.pos);
	}

	t.pos = { 0, 0, 0 };
	t.angle = { 0, 0, 0 };
	t.scale = { 1, 1, 1 };
}

