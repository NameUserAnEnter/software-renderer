#ifndef ENGINE_H
#define ENGINE_H

#include "Graphics.h"
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

	int2 VertexToPixel(Vertex, Transformation);
	int2 VertexToPixel(Vertex);

	// Draw topology methods used by RenderScene()
	void DrawPointList(Mesh);

	void DrawLineList(Mesh);
	void DrawLineStrip(Mesh);

	void DrawTriangleList(Mesh);
	void DrawTriangleStrip(Mesh);

public:
	int windowX, windowY;

	unsigned int uWindowWidth;
	unsigned int uWindowHeight;
	std::wstring windowTitle;

	ColorBlock drawingColor;
	ColorBlock backgroundColor;

private:
	HWND hWindow;
	
	bool bStop;

	Graphics graphics;
	Scene scene;
	Viewport viewport;

	std::string output;

	float3 saved_pos;
	float3 saved_angle;

	enum TOPOLOGIES {
		POINT_LIST,
		LINE_LIST, LINE_STRIP,
		TRIANGLE_LIST, TRIANGLE_STRIP
	} current_topology;
};

#endif

