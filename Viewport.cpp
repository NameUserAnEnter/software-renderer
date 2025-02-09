#include "Viewport.h"

int2 Viewport::ToScreen(float3 world) {
	int2 screen;
	//world.x                                                 = (screen.x - uViewportWidth / 2.f) / (uViewportWidth / 2.f);
	//world.x * (uViewportWidth / 2.f)                        = screen.x - uViewportWidth / 2.f
	//world.x * (uViewportWidth / 2.f) + uViewportWidth / 2.f = screen.x

	//screen.x = world.x * (uViewportWidth / 2.f) + uViewportWidth / 2.f;
	//screen.y = world.y * (uViewportHeight / 2.f) + uViewportHeight /2.f;

	world.x *= (viewportSize.y / viewportSize.x);
	world.z *= -1;

	screen.x = (world.x * (world.z * FOV + z_offset)) * (viewportSize.x / 2) + viewportSize.x / 2;
	screen.y = (world.y * (world.z * FOV + z_offset)) * (viewportSize.y / 2 * -1) + viewportSize.y /2;

	return screen;
}

float3 Viewport::ToWorld(int2 screen) {
	return ToWorld(screen, 0.0f);
}

float3 Viewport::ToWorld(int2 screen, float z) {
	//float3 world;
	//world.x = (screen.x - uViewportWidth / 2.f) / (uViewportWidth / 2.f);
	//world.y = (screen.y - uViewportHeight / 2.f) / (uViewportHeight / 2.f) * (-1);
	//world.z = z;

	//return world;
	
	return { 0, 0, 0 };
}

