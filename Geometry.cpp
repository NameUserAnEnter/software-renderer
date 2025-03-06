#include "Geometry.h"

void Geometry::Translate(float3& origin, float3 offset) {
	origin.x += offset.x;
	origin.y += offset.y;
	origin.z += offset.z;
}

void Geometry::Scale(float3& vertex, float3 scale) {
	vertex.x *= scale.x;
	vertex.y *= scale.y;
	vertex.z *= scale.z;
}

void Geometry::Scale(float3& vertex, float factor) {
	vertex.x *= factor;
	vertex.y *= factor;
	vertex.z *= factor;
}

void Geometry::ScaleX(float3& vertex, float factor) {
	vertex.x *= factor;
}

void Geometry::ScaleY(float3& vertex, float factor) {
	vertex.y *= factor;
}

void Geometry::ScaleZ(float3& vertex, float factor) {
	vertex.z *= factor;
}

void Geometry::RotateAroundAxisX(float3& vertex, float angle) {
	double radius = sqrt(pow(vertex.z, 2) + pow(vertex.y, 2));
	if (radius == 0) return;

	double vertex_angle, angle_to_x_axis, ratio;
	ratio = abs(vertex.z) / radius;
	angle_to_x_axis = acos(ratio);

	if (vertex.z >= 0 && vertex.y < 0)	// IV quadrant
	{
		vertex_angle = 2 * PI - angle_to_x_axis;
	}
	else if (vertex.z < 0 && vertex.y < 0)	// III quadrant
	{
		vertex_angle = PI + angle_to_x_axis;
	}
	else if (vertex.z < 0 && vertex.y >= 0) // II quadrant
	{
		vertex_angle = PI - angle_to_x_axis;
	}
	else	// I quadrant
	{
		vertex_angle = angle_to_x_axis;
	}

	double final_angle = vertex_angle - angle;

	vertex.z = cos(final_angle) * radius;
	vertex.y = sin(final_angle) * radius;
}

void Geometry::RotateAroundAxisY(float3& vertex, float angle) {
	double radius = sqrt(pow(vertex.z, 2) + pow(vertex.x, 2));
	if (radius == 0) return;

	double vertex_angle, angle_to_x_axis, ratio;
	ratio = abs(vertex.z) / radius;
	angle_to_x_axis = acos(ratio);

	if (vertex.z >= 0 && vertex.x < 0)	// IV quadrant
	{
		vertex_angle = 2 * PI - angle_to_x_axis;
	}
	else if (vertex.z < 0 && vertex.x < 0)	// III quadrant
	{
		vertex_angle = PI + angle_to_x_axis;
	}
	else if (vertex.z < 0 && vertex.x >= 0) // II quadrant
	{
		vertex_angle = PI - angle_to_x_axis;
	}
	else	// I quadrant
	{
		vertex_angle = angle_to_x_axis;
	}

	double final_angle = vertex_angle + angle;

	vertex.z = cos(final_angle) * radius;
	vertex.x = sin(final_angle) * radius;
}

void Geometry::RotateAroundAxisZ(float3& vertex, float angle) {
	double radius = sqrt(pow(vertex.x, 2) + pow(vertex.y, 2));
	if (radius == 0) return;

	double vertex_angle, angle_to_x_axis, ratio;
	ratio = abs(vertex.x) / radius;
	angle_to_x_axis = acos(ratio);

	if (vertex.x >= 0 && vertex.y < 0)	// IV quadrant
	{
		vertex_angle = 2 * PI - angle_to_x_axis;
	}
	else if (vertex.x < 0 && vertex.y < 0)	// III quadrant
	{
		vertex_angle = PI + angle_to_x_axis;
	}
	else if (vertex.x < 0 && vertex.y >= 0) // II quadrant
	{
		vertex_angle = PI - angle_to_x_axis;
	}
	else	// I quadrant
	{
		vertex_angle = angle_to_x_axis;
	}

	double final_angle = vertex_angle - angle;

	vertex.x = cos(final_angle) * radius;
	vertex.y = sin(final_angle) * radius;
}

bool Geometry::LineSegmentsCross(float2 a1, float2 a2, float2 b1, float2 b2) {
	if (fmax(a1.x, a2.x) < fmin(b1.x, b2.x) || (fmax(b1.x, b2.x) < fmin(a1.x, a2.x))) return false;
}

