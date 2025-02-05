#include "Geometry.h"

float2 Geometry::ToScreen(float3 world) {
	float2 screen;
	//world.x                                                 = (screen.x - uViewportWidth / 2.f) / (uViewportWidth / 2.f);
	//world.x * (uViewportWidth / 2.f)                        = screen.x - uViewportWidth / 2.f
	//world.x * (uViewportWidth / 2.f) + uViewportWidth / 2.f = screen.x
	
	//screen.x = world.x * (uViewportWidth / 2.f) + uViewportWidth / 2.f;
	//screen.y = world.y * (uViewportHeight / 2.f) + uViewportHeight /2.f;

	world.x *= (uViewportHeight / (float) uViewportWidth);
	world.z *= -1;

	// To do:
	// With perspective enabled,
	// vertices after some max distance is reached should
	// disappear, collapse into the center instead of escaping it again

	screen.x = (world.x * (world.z * FOV + z_offset)) * (uViewportWidth / 2.f) + uViewportWidth / 2.f;
	screen.y = (world.y * (world.z * FOV + z_offset)) * (uViewportHeight / 2.f * -1) + uViewportHeight /2.f;


	return screen;
}

float3 Geometry::ApproxWorldCoordinates(float2 screen) {
	//// To do: inspect the formulas and make sure they get exact with x and y
	// 
	//float3 world;
	//world.x = (screen.x - uViewportWidth / 2.f) / (uViewportWidth / 2.f);
	//world.y = (screen.y - uViewportHeight / 2.f) / (uViewportHeight / 2.f) * (-1);
	//world.z = 0.0f;

	//return world;

	return { 0, 0, 0 };
}

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

