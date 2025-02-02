#include "Geometry.h"

point2 Geometry::ToScreen(point3 world) {
	point2 screen;
	//world.x                                                 = (screen.x - uViewportWidth / 2.f) / (uViewportWidth / 2.f);
	//world.x * (uViewportWidth / 2.f)                        = screen.x - uViewportWidth / 2.f
	//world.x * (uViewportWidth / 2.f) + uViewportWidth / 2.f = screen.x
	
	//screen.x = world.x * (uViewportWidth / 2.f) + uViewportWidth / 2.f;
	//screen.y = world.y * (uViewportHeight / 2.f) + uViewportHeight /2.f;

	world.x *= (uViewportHeight / (float) uViewportWidth);

	static const float c = 0.05;
	screen.x = (world.x * (world.z * c + FOV)) * (uViewportWidth / 2.f) + uViewportWidth / 2.f;
	screen.y = (world.y * (world.z * c + FOV)) * (uViewportHeight / 2.f * -1) + uViewportHeight /2.f;


	return screen;
}

point3 Geometry::ToWorld(point2 screen) {
	point3 world;
	world.x = (screen.x - uViewportWidth / 2.f) / (uViewportWidth / 2.f);
	world.y = (screen.y - uViewportHeight / 2.f) / (uViewportHeight / 2.f) * (-1);
	world.z = 0.0f;

	return world;
}

point3 Geometry::Translate(point3 origin, point3 offset) {
	return { origin.x + offset.x, origin.y + offset.y, origin.z + offset.z };
}

point3 Geometry::Scale(point3 vertex, float factor) {
	return { vertex.x * factor, vertex.y * factor, vertex.z * factor };
}

