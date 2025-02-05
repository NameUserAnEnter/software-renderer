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

	void Update();

	void StopEngine();
	bool Done();

	void OnWindowResize(unsigned int, unsigned int);
private:
	void InitCustomScene();
	void InitModels();

	void ReadUserInput();
	void UpdateOutput();
	void RenderScene();

	float2 VertexToPixel(float3, Model);

	HWND hWindow;
	
	bool bStop;

	Scene scene;
	Graphics graphics;

	ColorBlockTransparent wireframeColor;

	std::string output;

	float3 saved_pos;
	float3 saved_angle;
};

#endif

