#ifndef ENGINE_H
#define ENGINE_H

#include "Graphics.h"
#include "Shape.h"
#include "Scene.h"
#include "Input.h"
#include "Geometry.h"
#include "Viewport.h"
#include "ColorDefs.h"
#include "utils.h"

class Engine {
public:
	Engine();

	void Init(HWND);
	void Release();

	void Update();

	void StopEngine();
	bool Done();

	float2 ViewportSize();

	void OnWindowResize(unsigned int, unsigned int);

private:
	void InitCustomScene();
	void InitModels();

	void ReadUserInput();
	void UpdateOutput();
	void RenderScene();

	float2 VertexToPixel(Vertex, Transformation);

public:
	int windowX, windowY;

	unsigned int uWindowWidth;
	unsigned int uWindowHeight;
	std::wstring windowTitle;

	ColorBlockTransparent wireframeColor;
	ColorBlockTransparent backgroundColor;

private:
	HWND hWindow;
	
	bool bStop;

	Graphics graphics;
	Scene scene;
	Viewport viewport;

	std::string output;

	float3 saved_pos;
	float3 saved_angle;
};

#endif

