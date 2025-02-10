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
		Vertex& vertex = vertices[i];

		Geometry::RotateAroundAxisY(vertex.pos, t.angle.y);
		Geometry::RotateAroundAxisX(vertex.pos, t.angle.x);
		Geometry::RotateAroundAxisZ(vertex.pos, t.angle.z);

		Geometry::Scale(vertex.pos, t.scale);

		Geometry::Translate(vertex.pos, t.pos);
	}

	t.pos = { 0, 0, 0 };
	t.angle = { 0, 0, 0 };
	t.scale = { 1, 1, 1 };
}

void Mesh::ApplyTransformationOnOutputBuffer() {
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

void Mesh::LoadWavefrontObj(std::wstring filepath) {
	std::vector<std::string> lines = SplitByChar(GetFileData(filepath.c_str()), '\n');

	std::vector<Vertex> vertices;
	std::vector<Face> faces;

	for (auto line : lines) {
		if (line.empty()) continue;

		if (line[0] == 'v') {						// if line starts with a 'v'
			auto tokens = SplitByChar(line, ' ');	// split line by spaces ' '
			tokens.erase(tokens.begin());			// remove 'v' prefix

			vertices.push_back({ 0, 0, 0 });

			for (int i = 0; i < 3 && i < tokens.size(); i++) {
				if (i == 0) vertices.back().pos.x = std::atof(tokens[i].c_str());
				if (i == 1) vertices.back().pos.y = std::atof(tokens[i].c_str());
				if (i == 2) vertices.back().pos.z = std::atof(tokens[i].c_str());
			}
		}
	}

	for (auto line : lines) {
		if (line.empty()) continue;

		if (line[0] == 'f') {						// if line starts with an 'f'
			auto tokens = SplitByChar(line, ' ');	// split line by spaces ' '
			tokens.erase(tokens.begin());			// remove 'f' prefix

			faces.push_back({});

			for (auto token : tokens) {
				auto subtokens = SplitByChar(token, '/');		// split a single vertex data by '/' into subtokens

				for (int i = 0; i < 3 && i < subtokens.size(); i++) {
					auto subtoken = subtokens[i];

					if (i == 1 && subtoken.empty()) continue;	// no texture index e.g. "f 1//1 2//2 3//3"

					if (i == 0) faces.back().vertexIndices.push_back(std::atoi(subtoken.c_str()));
					if (i == 1) faces.back().textureIndices.push_back(std::atoi(subtoken.c_str()));
					if (i == 2) faces.back().normalIndices.push_back(std::atoi(subtoken.c_str()));

				}
			}
		}
	}

	for (int i = 1; i < faces.size(); i++) {
		if (faces[i].vertexIndices.size() != faces[i - 1].vertexIndices.size()) {
			Popup(L"Inconsistent number of vertices per face: \"" + filepath + L"\"");
			return;
		}
	}

	for (auto face : faces) {
		if (face.vertexIndices.size() == 3)			topology = TRIANGLE_LIST;
		else if (face.vertexIndices.size() == 4)	topology = QUAD_LIST;
		else										topology = POINT_LIST;

		break;
	}

	for (auto face : faces) {
		for (auto vertexIndex : face.vertexIndices) {
			if (vertexIndex < 0) vertexIndex = vertices.size() + (vertexIndex + 1);	// convert negative indices to positive

			vertexIndex -= 1;		// Wavefront .obj face vertex indices are counted from 1 up

			if (vertexIndex < 0 || vertexIndex >= vertices.size()) {
				Popup(L"Invalid vertex index in face data: \"" + filepath + L"\"");
				return;
			}

			AddVertex(vertices[vertexIndex]);
		}
	}
}

