#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "DimensionalTypes.h"

class Viewport {
public:
	// Vertex pipeline methods
	void VertexAspectTransformation(float3&);
	void VertexPerspectiveTransformation(float3&);
	void VertexScreenTransformation(float3&);

public:
	float z_offset;
	float FOV;

	float2 viewportSize;
};

#endif

