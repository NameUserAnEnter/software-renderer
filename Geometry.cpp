#include "Geometry.h"

float3 Geometry::Translate(float3 origin, float3 offset) {
	return { origin.x + offset.x, origin.y + offset.y, origin.z + offset.z };
}

float3 Geometry::Scale(float3 vertex, float3 scale) {
	return {
		vertex.x * scale.x,
		vertex.y * scale.y,
		vertex.z * scale.z
	};
}

float3 Geometry::Scale(float3 vertex, float factor) {
	return {
		vertex.x * factor,
		vertex.y * factor,
		vertex.z * factor
	};
}

float3 Geometry::ScaleX(float3 vertex, float factor) {
	return {
		vertex.x * factor,
		vertex.y,
		vertex.z
	};
}

float3 Geometry::ScaleY(float3 vertex, float factor) {
	return {
		vertex.x,
		vertex.y * factor,
		vertex.z
	};
}

float3 Geometry::ScaleZ(float3 vertex, float factor) {
	return {
		vertex.x,
		vertex.y,
		vertex.z * factor
	};
}

float3 Geometry::RotateAroundAxisX(float3 vertex, float angle) {
	float2 pos = { vertex.z, vertex.y };

	double radius = sqrt(pow(pos.x, 2) + pow(pos.y, 2));
	if (radius == 0) return vertex;

	double vertex_angle, angle_to_x_axis, ratio;
	ratio = abs(pos.x) / radius;
	angle_to_x_axis = acos(ratio);

	if (pos.x >= 0 && pos.y < 0)	// IV quadrant
	{
		vertex_angle = 2 * PI - angle_to_x_axis;
	}
	else if (pos.x < 0 && pos.y < 0)	// III quadrant
	{
		vertex_angle = PI + angle_to_x_axis;
	}
	else if (pos.x < 0 && pos.y >= 0) // II quadrant
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

	return vertex;
}

float3 Geometry::RotateAroundAxisY(float3 vertex, float angle) {
	float2 pos = { vertex.z, vertex.x };

	double radius = sqrt(pow(pos.x, 2) + pow(pos.y, 2));
	if (radius == 0) return vertex;

	double vertex_angle, angle_to_x_axis, ratio;
	ratio = abs(pos.x) / radius;
	angle_to_x_axis = acos(ratio);

	if (pos.x >= 0 && pos.y < 0)	// IV quadrant
	{
		vertex_angle = 2 * PI - angle_to_x_axis;
	}
	else if (pos.x < 0 && pos.y < 0)	// III quadrant
	{
		vertex_angle = PI + angle_to_x_axis;
	}
	else if (pos.x < 0 && pos.y >= 0) // II quadrant
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

	return vertex;
}

float3 Geometry::RotateAroundAxisZ(float3 vertex, float angle) {
	float2 pos = { vertex.x, vertex.y };

	double radius = sqrt(pow(pos.x, 2) + pow(pos.y, 2));
	if (radius == 0) return vertex;

	double vertex_angle, angle_to_x_axis, ratio;
	ratio = abs(pos.x) / radius;
	angle_to_x_axis = acos(ratio);

	if (pos.x >= 0 && pos.y < 0)	// IV quadrant
	{
		vertex_angle = 2 * PI - angle_to_x_axis;
	}
	else if (pos.x < 0 && pos.y < 0)	// III quadrant
	{
		vertex_angle = PI + angle_to_x_axis;
	}
	else if (pos.x < 0 && pos.y >= 0) // II quadrant
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

	return vertex;
}

