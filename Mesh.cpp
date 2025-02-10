#include "Mesh.h"

Mesh::Mesh() {
	t.pos = { 0, 0, 0 };
	t.angle = { 0, 0, 0 };
	t.scale = { 1, 1, 1 };

	vertices = (Vertex*) calloc(0, sizeof(Vertex));
	outputBuffer = (Vertex*) calloc(0, sizeof(Vertex));

	cVertices = 0;

	topology = POINT_LIST;
}

Mesh::~Mesh() {
	Release();
}

void Mesh::Release() {
	free(vertices);
	free(outputBuffer);

	vertices = nullptr;
	outputBuffer = nullptr;

	cVertices = 0;
}

void Mesh::AddVertex(Vertex vertex) {
	Vertex* new_ptr;
	
	new_ptr = (Vertex*) realloc(vertices, (size_t) (sizeof(Vertex) * (cVertices + 1)));
	if (new_ptr == nullptr) return;
	vertices = new_ptr;

	new_ptr = (Vertex*) realloc(outputBuffer, (size_t) (sizeof(Vertex) * (cVertices + 1)));
	if (new_ptr == nullptr) return;
	outputBuffer = new_ptr;

	cVertices = cVertices + 1;
	vertices[cVertices - 1] = vertex;
}

unsigned int Mesh::GetVertexCount() {
	return cVertices;
}

void Mesh::ApplyTransformation() {
	for (int i = 0; i < cVertices; i++) {
		outputBuffer[i] = vertices[i];
		Vertex& vertex = outputBuffer[i];

		Geometry::RotateAroundAxisY(vertex.pos, t.angle.y);
		Geometry::RotateAroundAxisX(vertex.pos, t.angle.x);
		Geometry::RotateAroundAxisZ(vertex.pos, t.angle.z);

		Geometry::Scale(vertex.pos, t.scale);

		Geometry::Translate(vertex.pos, t.pos);
	}
}

