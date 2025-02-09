#ifndef GEOMETRY_H
#define GEOMETRY_H

#define PI 3.14159

#include "DimensionalTypes.h"
#include "utils.h"
#include <math.h>

struct Geometry {
	static void Translate(float3&, float3);

	static void Scale(float3&, float3);
	static void Scale(float3&, float);
	static void ScaleX(float3&, float);
	static void ScaleY(float3&, float);
	static void ScaleZ(float3&, float);

	static void RotateAroundAxisX(float3&, float);
	static void RotateAroundAxisY(float3&, float);
	static void RotateAroundAxisZ(float3&, float);
};

#endif

