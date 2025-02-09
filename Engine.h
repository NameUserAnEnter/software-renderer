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

	// Vertex pipeline methods, consider moving to viewport class as non-static methods
	static void VertexAspectTransformation(float3&, float2);
	static void VertexAspectTransformationReverse(float3&, float2);

	static void VertexPerspectiveTransformation(float3&, float, float);
	static void VertexPerspectiveTransformationReverse(float3&, float, float);

	static void VertexScreenTransformation(float3&, float2);
	static void VertexScreenTransformationReverse(float3&, float2);

	void MeshFullTransformation(Vertex*, unsigned int);
	void MeshFullTransformationReverse(Vertex*, unsigned int);

	// Draw topology methods used by RenderScene()
	void DrawPointList(Vertex*, unsigned int);

	void DrawLineList(Vertex*, unsigned int);
	void DrawLineStrip(Vertex*, unsigned int);

	void DrawTriangleList(Vertex*, unsigned int);
	void DrawTriangleStrip(Vertex*, unsigned int);

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
		UNDEFINED
	} current_topology;

	bool bWireframe;		// has impact only on triangle topologies
};

#endif

