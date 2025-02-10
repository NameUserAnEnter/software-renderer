#ifndef ENGINE_H
#define ENGINE_H

#include "Graphics.h"
#include "MeshElements.h"
#include "Mesh.h"
#include "Scene.h"
#include "Geometry.h"
#include "Viewport.h"
#include "ColorDefs.h"
#include "Input.h"
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

	void VerticesToScreen(Vertex*, unsigned int);

	// Draw topology methods used by RenderScene()
	void DrawPointList(Vertex*, unsigned int);

	void DrawLineList(Vertex*, unsigned int);
	void DrawLineStrip(Vertex*, unsigned int);

	void DrawTriangleList(Vertex*, unsigned int);
	void DrawTriangleStrip(Vertex*, unsigned int);

	void DrawQuadList(Vertex*, unsigned int);

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
		TRIANGLE_LIST, TRIANGLE_STRIP,
		QUAD_LIST,
		UNDEFINED, UNDEFINED2
	} current_topology;

	bool bWireframe;		// has impact only on polygonal topologies
};

#endif

