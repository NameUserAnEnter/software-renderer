#ifndef ENGINE_H
#define ENGINE_H

#include "Graphics.h"
#include "Shape.h"
#include "Scene.h"
#include "Input.h"
#include "Geometry.h"
#include "ColorDefs.h"
#include "utils.h"

class Engine {
public:
	void Init(HWND);
	void Release();

	bool Update();

	void OnWindowResize(unsigned int, unsigned int);
private:
	void InitCustomScene();
	void InitObjects();

	void MoveObjects();
	void ReadInputs();
	void RenderScene();

	HWND hWindow;

	bool breakOut;

	Scene scene;
	Graphics graphics;

	ColorBlockTransparent wireframeColor;

	std::string output;

	float3 saved_pos;
	float3 saved_angle;
};

#endif

