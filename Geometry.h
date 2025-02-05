#ifndef GEOMETRY_H
#define GEOMETRY_H

#define PI 3.14159

#include "Float.h"
#include "utils.h"
#include <math.h>

struct Geometry {
	static float3 Translate(float3, float3);

	static float3 Scale(float3, float3);
	static float3 Scale(float3, float);
	static float3 ScaleX(float3, float);
	static float3 ScaleY(float3, float);
	static float3 ScaleZ(float3, float);

	static float3 RotateAroundAxisX(float3, float);
	static float3 RotateAroundAxisY(float3, float);
	static float3 RotateAroundAxisZ(float3, float);
};

#endif

