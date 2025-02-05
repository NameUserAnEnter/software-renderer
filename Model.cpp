#include "Model.h"

Model::Model() {
	pos = { 0.0, 0.0, 0.0 };
	scale = 1.0;
	angle = { 0.0, 0.0, 0.0 };
}

void Model::AddPoint(float3 point) {
	for (auto p : points) {
		if (p == point) return;			// the point set does not contain duplicates
	}

	points.push_back(point);
}

void Model::AddPoint(float x, float y, float z) {
	AddPoint({ x, y, z });
}

std::vector<float3> Model::Points() {
	return points;
}

void Model::Release() {
}

