#include "Engine.h"

float Geometry::z_offset = 1;
float Geometry::FOV = 0.06;
unsigned int Geometry::uViewportWidth = 1280;
unsigned int Geometry::uViewportHeight = 960;

void Engine::Init(HWND hWindow) {
	this->hWindow = hWindow;

	bStop = false;

	RECT ClientRect;
	GetClientRect(hWindow, &ClientRect);

	unsigned int uClientWidth = ClientRect.right;
	unsigned int uClientHeight = ClientRect.bottom;

	Geometry::uViewportWidth = uClientWidth;
	Geometry::uViewportHeight = uClientHeight;

	graphics.Init(hWindow);
	graphics.ResizeBuffers(uClientWidth, uClientHeight);		// also calls InitializeBuffers()
	graphics.SetRasterUnitThickness(5);

	wireframeColor = Color::white;

	InitCustomScene();
}

void Engine::Release() {
	scene.End();
	graphics.ReleaseBuffers();
}

void Engine::Update() {
	// Logic
	ReadUserInput();
	Input::UpdateInputs();

	UpdateOutput();

	// Clear backbuffer
	graphics.ClearBackBuffer();

	// Render geometry
	RenderScene();

	// Swap buffers
	graphics.UpdateFrontBuffer();
}

void Engine::StopEngine() {
	bStop = true;
}

bool Engine::Done() {
	return bStop;
}

void Engine::OnWindowResize(unsigned int uNewClientWidth, unsigned int uNewClientHeight) {
	graphics.ResizeBuffers(uNewClientWidth, uNewClientHeight);

	Geometry::uViewportWidth = uNewClientWidth;
	Geometry::uViewportHeight = uNewClientHeight;
}

void Engine::InitCustomScene() {
	InitModels();

	Model& cube = scene.models.back();

	cube.scale = 1;
	cube.pos = { 0, 0, 0 };
	cube.angle = { 0, 0, 0 };

	cube.scale = 0.6;
	cube.pos = { 0, 0, 0 };
	cube.angle = { PI / 8, 0, 0 };
	//cube.angle = { PI / 8, PI / 6, 0 };

	saved_pos = cube.pos;
	saved_angle = cube.angle;
}

void Engine::InitModels() {
	Model cube;

	float3 v111 = { -1,  1, -1 };
	float3 v211 = {  1,  1, -1 };
	float3 v121 = { -1, -1, -1 };
	float3 v221 = {  1, -1, -1 };

	float3 v112 = { -1,  1, 1 };
	float3 v212 = {  1,  1, 1 };
	float3 v122 = { -1, -1, 1 };
	float3 v222 = {  1, -1, 1 };

	//		     112__________________________________212
	//		       /|                                /
	//		      / |                               /|
	//		     /  |                              / |
	//		    /   |                             /  |
	//		   /    |                            /   |
	//		  /     |                           /    |
	//    111/______|________________________211     |
	//		|       |                         |      |
	//		|       |                         |      |
	//		|       |                         |      |
	//		|       |                         |      |
	//		|       |                         |      |
	//		|       |                         |      |
	//		|    122|_________________________|______|222
	//		|      /                          |      /
	//		|     /                           |     /
	//		|    /                            |    /
	//		|   /                             |   /
	//		|  /                              |  /
	//		| /                               | /
	//		|/________________________________|/
	//    121                                221
	//

	cube.AddPoint(v111);
	cube.AddPoint(v211);
	cube.AddPoint(v121);
	cube.AddPoint(v221);

	cube.AddPoint(v112);
	cube.AddPoint(v212);
	cube.AddPoint(v122);
	cube.AddPoint(v222);

	scene.Begin();
	scene.AddModel(cube);
}

void Engine::ReadUserInput() {
	if (Input::Esc) StopEngine();

	Model& controlled = scene.models.back();

	float delta_pos = 0.01;
	float delta_scroll = 0.01;
	float delta_angle = (PI / 4) * 0.01;

	if (Input::Shift) delta_angle *= 5;

	if (Input::Alpha[Q]) controlled.angle.y -= delta_angle;
	if (Input::Alpha[E]) controlled.angle.y += delta_angle;

	if (Input::Alpha[W]) controlled.pos.z += delta_pos;
	if (Input::Alpha[S]) controlled.pos.z -= delta_pos;

	if (Input::Alpha[A]) controlled.pos.x -= delta_pos;
	if (Input::Alpha[D]) controlled.pos.x += delta_pos;

	if (Input::Alpha[R]) {
		controlled.pos = saved_pos;
		controlled.angle = saved_angle;
	}

	// To do: implement sliders to change those values and/or buttons to change scroll mode
	std::vector<float*> scrollable_values;
	scrollable_values.push_back(&controlled.scale);
	scrollable_values.push_back(&Geometry::z_offset);
	scrollable_values.push_back(&Geometry::FOV);

	static unsigned int scroll_mode = 0;

	if (Input::Alpha[Z]) scroll_mode = 0;
	if (Input::Alpha[X]) scroll_mode = 1;
	if (Input::Alpha[C]) scroll_mode = 2;

	output.clear();
	output += "Scroll mode: " + NumStr(scroll_mode) + ", ";

	if (Input::Shift) delta_scroll *= 10;

	if (Input::Mouse[mouse_control::SCROLL_DOWN])	*scrollable_values[scroll_mode] -= delta_scroll;
	if (Input::Mouse[mouse_control::SCROLL_UP])		*scrollable_values[scroll_mode] += delta_scroll;
}

void Engine::UpdateOutput() {
	Model cube = scene.models.back();

	//output.clear();
	output += "cube pos: ("		+ NumStr(cube.pos.x) + ", " + NumStr(cube.pos.y) + ", " + NumStr(cube.pos.z) + "), ";
	output += "cube scale: "	+ NumStr(cube.scale) + ", ";
	output += "z_offset: "		+ NumStr(Geometry::z_offset) + ", ";
	output += "FOV: "			+ NumStr(Geometry::FOV);

	// To do: Use project13 text rendering
	SetWindowTitle(hWindow, output);
}

void Engine::RenderScene() {
	for (Model model : scene.models) {
		std::vector<float3> points = model.Points();

		for (int i = 2; i < points.size(); i++) {
			float3 v0 = points[i - 2];
			float3 v1 = points[i - 1];
			float3 v2 = points[i];

			// To do:
			// Fix transformation pipeline order
			// Geometry struct needs new methods like PerspectiveTransformation and PerspectiveTransformationReverse, AspectTransformation, ..., and so on
			// Then these methods should be called at the right moment in the pipeline, either by ToScreen or somewhere else
			// 
			// Otherwise the cube is going to be rendered malformed because of e.g. perspective variables being used in the wrong place
			//
			// To do: implement camera position, rotation and turn

			float2 p0 = VertexToPixel(v0, model);
			float2 p1 = VertexToPixel(v1, model);
			float2 p2 = VertexToPixel(v2, model);
			
			ColorBlockTransparent color = wireframeColor;
			if (i < 3) color = Color::cyan;		// highlight something on the front face

			graphics.DrawTriangle(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, color);
		}
	}

	//graphics.DrawRectangle(20, 20, 140, 70, Color::cyan);
	//graphics.DrawTriangle(20, 160, 140, 200, 20, 100, Color::cyan);
	//graphics.DrawQuad(20, 220, 100, 320, 70, 520, 150, 550, Color::cyan);
}

float2 Engine::VertexToPixel(float3 vertex, Model model) {
	vertex = Geometry::RotateAroundAxisY(vertex, model.angle.y);
	vertex = Geometry::RotateAroundAxisX(vertex, model.angle.x);
	vertex = Geometry::RotateAroundAxisZ(vertex, model.angle.z);

	vertex = Geometry::Scale(vertex, model.scale);

	vertex= Geometry::Translate(vertex, model.pos);

	return Geometry::ToScreen(vertex);
}

