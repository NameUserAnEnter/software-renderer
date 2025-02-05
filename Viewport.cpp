#include "Viewport.h"

float2 Viewport::ToScreen(float3 world) {
	float2 screen;
	//world.x                                                 = (screen.x - uViewportWidth / 2.f) / (uViewportWidth / 2.f);
	//world.x * (uViewportWidth / 2.f)                        = screen.x - uViewportWidth / 2.f
	//world.x * (uViewportWidth / 2.f) + uViewportWidth / 2.f = screen.x

	//screen.x = world.x * (uViewportWidth / 2.f) + uViewportWidth / 2.f;
	//screen.y = world.y * (uViewportHeight / 2.f) + uViewportHeight /2.f;

	world.x *= (uViewportHeight / uViewportWidth);
	world.z *= -1;

	// To do:
	// With perspective enabled,
	// vertices after some max distance is reached should
	// disappear, collapse into the center instead of escaping it again

	screen.x = (world.x * (world.z * FOV + z_offset)) * (uViewportWidth / 2.f) + uViewportWidth / 2.f;
	screen.y = (world.y * (world.z * FOV + z_offset)) * (uViewportHeight / 2.f * -1) + uViewportHeight /2.f;


	return screen;
}

float3 Viewport::ApproxWorldCoordinates(float2 screen) {
	//// To do: inspect the formulas and make sure they get exact with x and y
	// 
	//float3 world;
	//world.x = (screen.x - uViewportWidth / 2.f) / (uViewportWidth / 2.f);
	//world.y = (screen.y - uViewportHeight / 2.f) / (uViewportHeight / 2.f) * (-1);
	//world.z = 0.0f;

	//return world;

	return { 0, 0, 0 };
}


