#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "DimensionalTypes.h"

struct Viewport {
	int2 ToScreen(float3);
	float3 ToWorld(int2);
	float3 ToWorld(int2, float);

	float z_offset;
	float FOV;

	float2 viewportSize;
};

#endif

