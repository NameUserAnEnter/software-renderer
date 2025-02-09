#include "Engine.h"

Engine::Engine() {
	bStop = false;

	windowX = 0;
	windowY = 0;

	uWindowWidth = 1280;
	uWindowHeight = 960;
	windowTitle = L"Preview";

	drawingColor	= Color::white;
	backgroundColor = Color::dark_gray;

	current_topology = TOPOLOGIES::LINE_LIST;
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
	//output += NumStr(viewport.fViewportWidth) + "x" + NumStr(viewport.fViewportHeight) + ", ";

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

	if (current_topology == POINT_LIST) {
		cube.AddVertex(v111);
		cube.AddVertex(v211);
		cube.AddVertex(v121);
		cube.AddVertex(v221);
		cube.AddVertex(v112);
		cube.AddVertex(v212);
		cube.AddVertex(v122);
		cube.AddVertex(v222);
	}
	else if (current_topology == LINE_LIST) {
		// front side
		cube.AddVertex(v111);
		cube.AddVertex(v211);

		cube.AddVertex(v211);
		cube.AddVertex(v221);

		cube.AddVertex(v221);
		cube.AddVertex(v111);

		cube.AddVertex(v111);
		cube.AddVertex(v221);

		cube.AddVertex(v221);
		cube.AddVertex(v121);

		cube.AddVertex(v121);
		cube.AddVertex(v111);

		// left side
		cube.AddVertex(v112);
		cube.AddVertex(v111);

		cube.AddVertex(v111);
		cube.AddVertex(v121);
		
		cube.AddVertex(v121);
		cube.AddVertex(v112);

		cube.AddVertex(v112);
		cube.AddVertex(v121);

		cube.AddVertex(v121);
		cube.AddVertex(v122);

		cube.AddVertex(v122);
		cube.AddVertex(v112);

		// back side
		cube.AddVertex(v212);
		cube.AddVertex(v112);

		cube.AddVertex(v112);
		cube.AddVertex(v122);

		cube.AddVertex(v122);
		cube.AddVertex(v212);

		cube.AddVertex(v212);
		cube.AddVertex(v122);

		cube.AddVertex(v122);
		cube.AddVertex(v222);

		cube.AddVertex(v222);
		cube.AddVertex(v212);

		// right side
		cube.AddVertex(v211);
		cube.AddVertex(v212);

		cube.AddVertex(v212);
		cube.AddVertex(v222);
		
		cube.AddVertex(v222);
		cube.AddVertex(v211);

		cube.AddVertex(v211);
		cube.AddVertex(v222);

		cube.AddVertex(v222);
		cube.AddVertex(v221);

		cube.AddVertex(v221);
		cube.AddVertex(v211);

		// top side
		cube.AddVertex(v112);
		cube.AddVertex(v212);

		cube.AddVertex(v212);
		cube.AddVertex(v211);

		cube.AddVertex(v211);
		cube.AddVertex(v112);

		cube.AddVertex(v112);
		cube.AddVertex(v211);

		cube.AddVertex(v211);
		cube.AddVertex(v111);

		cube.AddVertex(v111);
		cube.AddVertex(v112);

		// bottom side
		cube.AddVertex(v121);
		cube.AddVertex(v221);

		cube.AddVertex(v221);
		cube.AddVertex(v222);

		cube.AddVertex(v222);
		cube.AddVertex(v121);

		cube.AddVertex(v121);
		cube.AddVertex(v222);

		cube.AddVertex(v222);
		cube.AddVertex(v122);

		cube.AddVertex(v122);
		cube.AddVertex(v121);
	}
	else if (current_topology == LINE_STRIP) {
	}
	else if (current_topology == TRIANGLE_LIST) {
	}
	else if (current_topology == TRIANGLE_STRIP) {
	}

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

	if (Input::Alpha[Q]) controlled.angle.y += delta_angle;
	if (Input::Alpha[E]) controlled.angle.y -= delta_angle;

	if (Input::Alpha[W]) controlled.angle.x += delta_angle;
	if (Input::Alpha[S]) controlled.angle.x -= delta_angle;

	//if (Input::Alpha[W]) controlled.pos.z += delta_pos;
	//if (Input::Alpha[S]) controlled.pos.z -= delta_pos;

	if (Input::Alpha[A]) controlled.pos.x -= delta_pos;
	if (Input::Alpha[D]) controlled.pos.x += delta_pos;

	if (Input::Alpha[R]) {
		controlled.pos = saved_pos;
		controlled.angle = saved_angle;
	}

	static float scale = controlled.scale.z;

	// To do: implement sliders to change those values and/or buttons to change scroll mode
	std::vector<float*> scrollable_values;
	scrollable_values.push_back(&scale);
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

	controlled.scale = { scale, scale, scale };
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
		// To do: implement z-buffering
		// To do: implement culling after z-buffering

		if (current_topology == POINT_LIST) DrawPointList(mesh);
		else if (current_topology == LINE_LIST) DrawLineList(mesh);
		else if (current_topology == LINE_STRIP) DrawLineStrip(mesh);
		else if (current_topology == TRIANGLE_LIST) DrawTriangleList(mesh);
		else if (current_topology == TRIANGLE_STRIP) DrawTriangleStrip(mesh);
	}

	// To do: add a grid plane
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

int2 Engine::VertexToPixel(Vertex vertex) {
	float3 pos = { vertex.x, vertex.y, vertex.z };
	return viewport.ToScreen(pos);
}

void Engine::DrawPointList(Mesh mesh) {
	std::vector<Vertex> vertices = mesh.Vertices();

	for (int i = 0; i < vertices.size(); i++) {
		Vertex v = vertices[i];
		int2 p = VertexToPixel(v, mesh.t);

		graphics.DrawPoint(p, drawingColor);
	}
}

void Engine::DrawLineList(Mesh mesh) {
	std::vector<Vertex> vertices = mesh.Vertices();

	// attention construction site here
	for (int i = 1; i < vertices.size(); i += 2) {
		Vertex v0 = vertices[i - 1];
		Vertex v1 = vertices[i];

		int2 p0 = VertexToPixel(v0, mesh.t);
		int2 p1 = VertexToPixel(v1, mesh.t);

		graphics.DrawLine(p0, p1, drawingColor);
	}
}

void Engine::DrawLineStrip(Mesh mesh) {
	std::vector<Vertex> vertices = mesh.Vertices();

	// attention construction site here
	for (int i = 0; i < vertices.size(); i++) {
	}
}

void Engine::DrawTriangleList(Mesh mesh) {
	std::vector<Vertex> vertices = mesh.Vertices();

	// attention construction site here
	for (int i = 0; i < vertices.size(); i++) {
	}
}

void Engine::DrawTriangleStrip(Mesh mesh) {
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
		// To do: implement a camera class

		// To do: implement z-buffering or any other hidden-surface/line-determination algorithm
		// To do: base mesh generation on the .obj format and implement .obj format mesh loading 

		int2 p0 = VertexToPixel(v0, mesh.t);
		int2 p1 = VertexToPixel(v1, mesh.t);
		int2 p2 = VertexToPixel(v2, mesh.t);

		ColorBlock color = drawingColor;
		if (i < 3) color = Color::cyan;		// temporary: highlight some polygon on the front face to test HSD/z-buffering

		graphics.FillTriangle(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, color);
	}
}

