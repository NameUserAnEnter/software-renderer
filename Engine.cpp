#include "Engine.h"

Engine::Engine() {
	bStop = false;

	windowX = 0;
	windowY = 0;

	uWindowWidth = 1280;
	uWindowHeight = 960;
	windowTitle = L"Preview";

	wireframeColor	= Color::white;
	backgroundColor = Color::dark_purple;
}

void Engine::Init(HWND hWnd) {
	if (!hWnd) {
		StopEngine();
		return;
	}

	hWindow = hWnd;

	RECT ClientRect;
	GetClientRect(hWindow, &ClientRect);

	viewport.z_offset = 1;
	viewport.FOV = 0.06;
	viewport.fViewportWidth = ClientRect.right;
	viewport.fViewportHeight = ClientRect.bottom;

	graphics.Init(hWindow);
	graphics.ResizeBuffers(ClientRect.right, ClientRect.bottom);		// also calls InitializeBuffers()
	//graphics.SetRasterUnitThickness(5);

	InitCustomScene();
}

void Engine::Release() {
	scene.End();
	graphics.ReleaseBuffers();
}

void Engine::Update() {
	output += NumStr(viewport.fViewportWidth) + "x" + NumStr(viewport.fViewportHeight) + ", ";

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

float2 Engine::ViewportSize() {
	return { viewport.fViewportWidth, viewport.fViewportHeight };
}

void Engine::OnWindowResize(unsigned int uNewClientWidth, unsigned int uNewClientHeight) {
	graphics.ResizeBuffers(uNewClientWidth, uNewClientHeight);

	viewport.fViewportWidth = uNewClientWidth;
	viewport.fViewportHeight = uNewClientHeight;
}

void Engine::InitCustomScene() {
	InitModels();

	Mesh& cube = scene.meshList.back();

	cube.t.scale = { 0.6, 0.6, 0.6 };
	cube.t.pos = { 0, 0, 0 };
	cube.t.angle = { PI / 8, 0, 0 };
	//cube.t.angle = { PI / 8, PI / 6, 0 };

	saved_pos = cube.t.pos;
	saved_angle = cube.t.angle;
}

void Engine::InitModels() {
	Mesh cube;

	Vertex v111 = { -1,  1, -1 };
	Vertex v211 = {  1,  1, -1 };
	Vertex v121 = { -1, -1, -1 };
	Vertex v221 = {  1, -1, -1 };

	Vertex v112 = { -1,  1, 1 };
	Vertex v212 = {  1,  1, 1 };
	Vertex v122 = { -1, -1, 1 };
	Vertex v222 = {  1, -1, 1 };

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

	cube.AddVertex(v111);
	cube.AddVertex(v211);
	cube.AddVertex(v121);
	cube.AddVertex(v221);

	cube.AddVertex(v112);
	cube.AddVertex(v212);
	cube.AddVertex(v122);
	cube.AddVertex(v222);

	scene.Begin();
	scene.AddMesh(cube);
}

void Engine::ReadUserInput() {
	if (Input::Esc) StopEngine();

	Transformation& controlled = scene.meshList.back().t;

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
	scrollable_values.push_back(&controlled.scale.z);
	scrollable_values.push_back(&viewport.z_offset);
	scrollable_values.push_back(&viewport.FOV);

	static unsigned int scroll_mode = 0;

	if (Input::Alpha[Z]) scroll_mode = 0;
	if (Input::Alpha[X]) scroll_mode = 1;
	if (Input::Alpha[C]) scroll_mode = 2;

	output += "scroll mode: " + NumStr(scroll_mode) + ", ";

	if (Input::Shift) delta_scroll *= 10;

	if (Input::Mouse[mouse_control::SCROLL_DOWN])	*scrollable_values[scroll_mode] -= delta_scroll;
	if (Input::Mouse[mouse_control::SCROLL_UP])		*scrollable_values[scroll_mode] += delta_scroll;
}

void Engine::UpdateOutput() {
	Transformation cube = scene.meshList.back().t;

	output += "cube pos: ("		+ NumStr(cube.pos.x) + ", " + NumStr(cube.pos.y) + ", " + NumStr(cube.pos.z) + "), ";
	output += "cube angle: ("	+ NumStr(cube.angle.x) + ", " + NumStr(cube.angle.y) + ", " + NumStr(cube.angle.z) + "), ";
	output += "cube scale: ("	+ NumStr(cube.scale.x) + ", " + NumStr(cube.scale.y) + ", " + NumStr(cube.scale.z) + "), ";
	output += "z_offset: "		+ NumStr(viewport.z_offset) + ", ";
	output += "FOV: "			+ NumStr(viewport.FOV);

	// To do: Use project13 text rendering
	SetWindowTitle(hWindow, output);
	output = "";
}

void Engine::RenderScene() {
	for (Mesh mesh : scene.meshList) {
		std::vector<Vertex> vertices = mesh.Vertices();

		for (int i = 2; i < vertices.size(); i++) {
			Vertex v0 = vertices[i - 2];
			Vertex v1 = vertices[i - 1];
			Vertex v2 = vertices[i];

			// To do:
			// Fix transformation pipeline order
			// Geometry struct needs new methods like PerspectiveTransformation and PerspectiveTransformationReverse, AspectTransformation, ..., and so on
			// Then these methods should be called at the right moment in the pipeline, either by ToScreen or somewhere else
			// 
			// Otherwise the cube is going to be rendered malformed because of e.g. perspective variables being used in the wrong place
			//
			// To do: implement camera position, rotation and turn

			int2 p0 = VertexToPixel(v0, mesh.t);
			int2 p1 = VertexToPixel(v1, mesh.t);
			int2 p2 = VertexToPixel(v2, mesh.t);
			
			ColorBlock color = wireframeColor;
			if (i < 3) color = Color::cyan;		// highlight something on the front face

			graphics.FillTriangle(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, color);
		}
	}

	//graphics.DrawRectangle(20, 20, 140, 70, Color::cyan);
	//graphics.DrawTriangle(20, 160, 140, 200, 20, 100, Color::cyan);
	//graphics.DrawQuad(20, 220, 100, 320, 70, 520, 150, 550, Color::cyan);
}

int2 Engine::VertexToPixel(Vertex vertex, Transformation t) {
	float3 pos = { vertex.x, vertex.y, vertex.z };

	pos = Geometry::RotateAroundAxisY(pos, t.angle.y);
	pos = Geometry::RotateAroundAxisX(pos, t.angle.x);
	pos = Geometry::RotateAroundAxisZ(pos, t.angle.z);

	pos = Geometry::Scale(pos, t.scale);

	pos= Geometry::Translate(pos, t.pos);

	return viewport.ToScreen(pos);
}

