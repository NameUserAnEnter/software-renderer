#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "Float.h"

struct Viewport {
	float2 ToScreen(float3);
	float3 ApproxWorldCoordinates(float2);

	float z_offset;
	float FOV;

	float uViewportWidth;
	float uViewportHeight;
};

#endif

