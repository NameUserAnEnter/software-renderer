#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Point.h"
#include <Windows.h>

struct Geometry {
	static point2 ToScreen(point3);
	static point3 ToWorld(point2);

	static point3 Translate(point3, point3);
	static point3 Scale(point3, float);

	static float FOV;

	static unsigned int uViewportWidth;
	static unsigned int uViewportHeight;
};

#endif

