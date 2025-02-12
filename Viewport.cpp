#include "Viewport.h"

void Viewport::VertexAspectTransformation(float3& pos) {
	pos.x *= (viewportSize.y / viewportSize.x);
}

void Viewport::VertexAspectTransformationReverse(float3& pos) {
	pos.x /= (viewportSize.y / viewportSize.x);
}

void Viewport::VertexPerspectiveTransformation(float3& pos) {
	//pos.z *= -1;

	pos.x *= -pos.z * FOV + z_offset;
	pos.y *= -pos.z * FOV + z_offset;
}

void Viewport::VertexPerspectiveTransformationReverse(float3& pos) {
	pos.y /= -pos.z * FOV + z_offset;
	pos.x /= -pos.z * FOV + z_offset;

	//pos.z *= -1;
}

void Viewport::VertexScreenTransformation(float3& pos) {
	pos.x *= (viewportSize.x / 2);
	pos.x += (viewportSize.x / 2);

	pos.y *= -1;
	pos.y *= (viewportSize.y / 2);
	pos.y += (viewportSize.y / 2);
}

void Viewport::VertexScreenTransformationReverse(float3& pos) {
	pos.y -= (viewportSize.y / 2);
	pos.y /= (viewportSize.y / 2);
	pos.y *= -1;

	pos.x -= (viewportSize.x / 2);
	pos.x /= (viewportSize.x / 2);
}

