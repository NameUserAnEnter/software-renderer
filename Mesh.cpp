#include "Mesh.h"

Mesh::Mesh() {
	t.pos = { 0, 0, 0 };
	t.angle = { 0, 0, 0 };
	t.scale = { 1, 1, 1 };

	vertices = (Vertex*) calloc(0, sizeof(Vertex));
	cVertices = 0;
}

Mesh::~Mesh() {
	Release();
}

void Mesh::Release() {
	free(vertices);

	vertices = nullptr;
	cVertices = 0;
}

void Mesh::AddVertex(Vertex vertex) {
	Vertex* new_ptr = (Vertex*) realloc(vertices, (size_t) (sizeof(Vertex) * (cVertices + 1)));
	if (new_ptr == nullptr) return;

	vertices = new_ptr;
	cVertices = cVertices + 1;

	vertices[cVertices - 1] = vertex;
}

unsigned int Mesh::GetVertexCount() {
	return cVertices;
}

void Mesh::ApplyTransformation() {
	for (int i = 0; i < cVertices; i++) {
		Vertex& vertex = vertices[i];

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

