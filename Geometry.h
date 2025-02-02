#ifndef GEOMETRY_H
#define GEOMETRY_H

#define PI 3.14159

#include "Float.h"
#include "utils.h"
#include <Windows.h>
#include <math.h>

struct Geometry {
	static float2 ToScreen(float3);
	static float3 ToWorld(float2);

	static float3 Translate(float3, float3);
	static float3 Scale(float3, float);
	static float3 RotateAroundAxisX(float3, float);
	static float3 RotateAroundAxisY(float3, float);
	static float3 RotateAroundAxisZ(float3, float);

	static float FOV;

	static unsigned int uViewportWidth;
	static unsigned int uViewportHeight;
};

#endif

