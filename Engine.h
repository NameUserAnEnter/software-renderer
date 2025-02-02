#ifndef ENGINE_H
#define ENGINE_H

#include "Graphics.h"
#include "Shape.h"
#include "Scene.h"
#include "Input.h"
#include "Geometry.h"
#include "utils.h"

class Engine {
public:
	void Init(HWND);
	void Release();

	bool Update();
	void OnKeystateChange(bool, unsigned long long);
	void OnWindowResize(unsigned int, unsigned int);
private:
	void InitCustomScene();

	void MoveObjects();
	void ReadInputs();
	void RenderScene();

	HWND hWindow;

	bool breakOut;

	Scene scene;
	Graphics graphics;

	ColorBlockTransparent wireframeColor;
};

#endif

