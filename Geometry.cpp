#include "Geometry.h"

point2 Geometry::ToScreen(point3 world) {
	point2 screen;
	//world.x                                                 = (screen.x - uViewportWidth / 2.f) / (uViewportWidth / 2.f);
	//world.x * (uViewportWidth / 2.f)                        = screen.x - uViewportWidth / 2.f
	//world.x * (uViewportWidth / 2.f) + uViewportWidth / 2.f = screen.x
	
	//screen.x = world.x * (uViewportWidth / 2.f) + uViewportWidth / 2.f;
	//screen.y = world.y * (uViewportHeight / 2.f) + uViewportHeight /2.f;

	screen.x = (world.x * (world.z + FOV)) * (uViewportWidth / 2.f) + uViewportWidth / 2.f;
	screen.y = (world.y * (world.z + FOV)) * (uViewportHeight / 2.f) + uViewportHeight /2.f;

	return screen;
}

point3 Geometry::ToWorld(point2 screen) {
	point3 world;
	world.x = (screen.x - uViewportWidth / 2.f) / (uViewportWidth / 2.f);
	world.y = (screen.y - uViewportHeight / 2.f) / (uViewportHeight / 2.f) * (-1);
	world.z = 0.0f;

	return world;
}

