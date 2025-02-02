#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Point.h"

class Geometry {
public:
	Geometry();

	point2 ToScreen(point3);
	point3 ToWorld(point2);

	static float FOV;
};

#endif

