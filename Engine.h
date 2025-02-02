#ifndef ENGINE_H
#define ENGINE_H

#include "Graphics.h"
#include "Shape.h"
#include "Scene.h"
#include "Geometry.h"
#include "Input.h"
#include "utils.h"

class Engine {
public:
	void Init(HWND);
	void Release();

	void Update();
	void OnKeyPress();
	void OnKeyRelease();
	void OnWindowResize(unsigned int, unsigned int);
private:
	void InitCustomScene();
	void RenderScene();

	HWND hWindow;

	Scene scene;
	Graphics graphics;

	ColorBlockTransparent wireframeColor;
};

#endif

