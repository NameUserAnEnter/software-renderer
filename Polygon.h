#ifndef POLYGON2_H
#define POLYGON2_H

#include "DimensionalTypes.h"

struct Polygon {
private:
	float2* vertices;
	unsigned int corners;

public:
	Polygon() {
		vertices = (float2*) calloc(0, sizeof(float2));
		corners = 0;
	}

	~Polygon() {
		free(vertices);
	}

	float2& operator[] (unsigned int index) {
		if (index >= corners) {
			float2* ptr = (float2*) realloc(vertices, sizeof(float2) * (corners + 1));

			if (ptr != nullptr) {
				vertices = ptr;
				corners++;
			}
		}

		return vertices[index];
	}
};

#endif

