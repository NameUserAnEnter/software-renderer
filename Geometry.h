#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Point.h"

struct Geometry {
	static point2 ToScreen(point3);
	static point3 ToWorld(point2);

	static float FOV;

	static unsigned int uViewportWidth;
	static unsigned int uViewportHeight;
};

#endif

