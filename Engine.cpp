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

	current_topology = TOPOLOGIES::TRIANGLE_LIST;

	bWireframe = true;
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
	viewport.viewportSize.x = ClientRect.right;
	viewport.viewportSize.y = ClientRect.bottom;

	graphics.Init(hWindow);
	graphics.ResizeBuffers(ClientRect.right, ClientRect.bottom);		// also calls InitializeBuffers()
	//graphics.SetRasterUnitThickness(5);

	scene.Begin();
	InitCustomScene();
}

void Engine::Release() {
	scene.End();
	graphics.ReleaseBuffers();
}

void Engine::Update() {
	output += NumStr(viewport.viewportSize.x) + "x" + NumStr(viewport.viewportSize.y) + ", ";

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
	return { viewport.viewportSize.x, viewport.viewportSize.y };
}

void Engine::OnWindowResize(unsigned int uNewClientWidth, unsigned int uNewClientHeight) {
	graphics.ResizeBuffers(uNewClientWidth, uNewClientHeight);

	viewport.viewportSize.x = uNewClientWidth;
	viewport.viewportSize.y = uNewClientHeight;
}

void Engine::InitCustomScene() {
	InitModels();

	if (scene.GetMeshCount() <= 0) return;
	Mesh& mesh = *scene.meshes[scene.GetMeshCount() - 1];

	mesh.t.scale = { 0.6, 0.6, 0.6 };
	mesh.t.pos = { 0, 0, 0 };
	mesh.t.angle = { PI / -8, 0, 0 };
	//t.angle = { PI / -8, PI / 6, 0 };

	saved_pos = mesh.t.pos;
	saved_angle = mesh.t.angle;
}

void Engine::InitModels() {
	scene.AddMesh();
	Mesh& cube = *scene.meshes[scene.GetMeshCount() - 1];

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
		// front side
		cube.AddVertex(v111);
		cube.AddVertex(v211);
		cube.AddVertex(v221);
		cube.AddVertex(v111);
		cube.AddVertex(v221);
		cube.AddVertex(v121);
		cube.AddVertex(v111);

		// to be continued, figured out, changed, fixed, or left undone
	}
	else if (current_topology == TRIANGLE_LIST) {
		// front side
		cube.AddVertex(v111);
		cube.AddVertex(v211);
		cube.AddVertex(v221);

		cube.AddVertex(v111);
		cube.AddVertex(v221);
		cube.AddVertex(v121);

		// left side
		cube.AddVertex(v112);
		cube.AddVertex(v111);
		cube.AddVertex(v121);

		cube.AddVertex(v112);
		cube.AddVertex(v121);
		cube.AddVertex(v122);

		// back side
		cube.AddVertex(v212);
		cube.AddVertex(v222);
		cube.AddVertex(v122);

		cube.AddVertex(v212);
		cube.AddVertex(v122);
		cube.AddVertex(v112);

		// right side
		cube.AddVertex(v211);
		cube.AddVertex(v212);
		cube.AddVertex(v222);

		cube.AddVertex(v211);
		cube.AddVertex(v222);
		cube.AddVertex(v221);

		// top side
		cube.AddVertex(v112);
		cube.AddVertex(v212);
		cube.AddVertex(v211);

		cube.AddVertex(v112);
		cube.AddVertex(v211);
		cube.AddVertex(v111);

		// bottom side
		cube.AddVertex(v121);
		cube.AddVertex(v221);
		cube.AddVertex(v222);

		cube.AddVertex(v121);
		cube.AddVertex(v222);
		cube.AddVertex(v122);
	}
	else if (current_topology == TRIANGLE_STRIP) {
		// front side
		cube.AddVertex(v211);
		cube.AddVertex(v221);
		cube.AddVertex(v111);

		cube.AddVertex(v121);

		// left side
		cube.AddVertex(v112);
		cube.AddVertex(v122);

		// back side
		cube.AddVertex(v212);
		cube.AddVertex(v222);

		// right side
		cube.AddVertex(v211);
		cube.AddVertex(v221);

		// incomplete...
	}
	else if (current_topology == UNDEFINED) {
		// data based on data/cube4.obj, interpreting data as LHS, no x-axis inversion

		// v 1.000000 1.000000 -1.000000
		cube.AddVertex({  1,  1, -1 });

		// v 1.000000 -1.000000 -1.000000
		cube.AddVertex({  1, -1, -1 });

		// v 1.000000 1.000000 1.000000
		cube.AddVertex({  1,  1,  1 });

		// v 1.000000 -1.000000 1.000000
		cube.AddVertex({  1, -1,  1 });

		// v -1.000000 1.000000 -1.000000
		cube.AddVertex({ -1,  1, -1 });

		// v -1.000000 -1.000000 -1.000000
		cube.AddVertex({ -1, -1, -1 });

		// v -1.000000 1.000000 1.000000
		cube.AddVertex({ -1,  1,  1 });

		// v -1.000000 -1.000000 1.000000
		cube.AddVertex({ -1, -1,  1 });

		// top side
		// f 1 5 7 3
		// 7 -- 3
		// |    |
		// 5 -- 1

		// back side
		// f 4 3 7 8
		// 7 -- 3
		// |    |
		// 8 -- 4

		// left side
		// f 8 7 5 6
		// 7 -- 5
		// |    |
		// 8 -- 6

		// bottom side
		// f 6 2 4 8
		// 6 -- 2
		// |    |
		// 8 -- 4

		// right side
		// f 2 1 3 4
		// 1 -- 3
		// |    |
		// 2 -- 4

		// front side
		// f 6 5 1 2
		// 5 -- 1
		// |    |
		// 6 -- 2
	}
}

void Engine::ReadUserInput() {
	if (Input::Esc) StopEngine();

	if (scene.GetMeshCount() <= 0) return;
	Mesh& mesh = *scene.meshes[scene.GetMeshCount() - 1];
	Transformation& controlled = mesh.t;

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

	//if (Input::Alpha[W]) controlled.pos.y += delta_pos;
	//if (Input::Alpha[S]) controlled.pos.y -= delta_pos;

	if (Input::Alpha[A]) controlled.pos.x -= delta_pos;
	if (Input::Alpha[D]) controlled.pos.x += delta_pos;

	if (Input::Alpha[R]) {
		controlled.pos = saved_pos;
		controlled.angle = saved_angle;
	}

	// this kind of construction makes sure the action is performed only once per keypress
	static bool keystate_k = false;
	if (Input::Alpha[K]) {
		if (!keystate_k) bWireframe = !bWireframe;
		keystate_k = true;
	}
	else keystate_k = false;

	static float scale = controlled.scale.z;

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
	if (scene.GetMeshCount() <= 0) return;
	Mesh& mesh = *scene.meshes[scene.GetMeshCount() - 1];
	Transformation& t = mesh.t;

	output += "cube pos: ("		+ NumStr(t.pos.x) + ", " + NumStr(t.pos.y) + ", " + NumStr(t.pos.z) + "), ";
	output += "cube angle: ("	+ NumStr(t.angle.x) + ", " + NumStr(t.angle.y) + ", " + NumStr(t.angle.z) + "), ";
	output += "cube scale: ("	+ NumStr(t.scale.x) + ", " + NumStr(t.scale.y) + ", " + NumStr(t.scale.z) + "), ";
	output += "z_offset: "		+ NumStr(viewport.z_offset) + ", ";
	output += "FOV: "			+ NumStr(viewport.FOV);
	
	if (mesh.GetVertexCount() > 0) {
		float3 p = mesh.vertices[0].pos;
		output += ", vertex0: ("	+ NumStr(p.x) + ", " + NumStr(p.y) + ", " + NumStr(p.z) + ")";
	}

	SetWindowTitle(hWindow, output);
	output = "";
}

void Engine::RenderScene() {
	for (int i = 0; i < scene.GetMeshCount(); i++) {
		Mesh& mesh = *scene.meshes[i];
		mesh.ApplyTransformation();

		Vertex* vertices = mesh.outputBuffer;
		unsigned int cVertices = mesh.GetVertexCount();

		MeshToScreen(vertices, cVertices);

		switch (current_topology) {
			case POINT_LIST:		DrawPointList(vertices, cVertices);		break;
			case LINE_LIST:			DrawLineList(vertices, cVertices);		break;
			case LINE_STRIP:		DrawLineStrip(vertices, cVertices);		break;
			case TRIANGLE_LIST:		DrawTriangleList(vertices, cVertices);	break;
			case TRIANGLE_STRIP:	DrawTriangleStrip(vertices, cVertices);	break;

			case UNDEFINED:			DrawPointList(vertices, cVertices);		break;
		}
	}
}

void Engine::MeshToScreen(Vertex* vertices, unsigned int cVertices) {
	for (int i = 0; i < cVertices; i++) {
		viewport.VertexAspectTransformation(vertices[i].pos);
		viewport.VertexPerspectiveTransformation(vertices[i].pos);
		viewport.VertexScreenTransformation(vertices[i].pos);
	}
}

void Engine::DrawPointList(Vertex* vertices, unsigned int cVertices) {
	for (int i = 0; i < cVertices; i++) {
		Vertex& v = vertices[i];
		int2 p = { v.pos.x, v.pos.y };

		graphics.DrawPoint(p, drawingColor);
	}
}

void Engine::DrawLineList(Vertex* vertices, unsigned int cVertices) {
	for (int i = 1; i < cVertices; i += 2) {
		Vertex& v0 = vertices[i - 1];
		Vertex& v1 = vertices[i];

		int2 p0 = { v0.pos.x, v0.pos.y };
		int2 p1 = { v1.pos.x, v1.pos.y };

		graphics.DrawLine(p0, p1, drawingColor);
	}
}

void Engine::DrawLineStrip(Vertex* vertices, unsigned int cVertices) {
	for (int i = 1; i < cVertices; i++) {
		Vertex& v0 = vertices[i - 1];
		Vertex& v1 = vertices[i];

		int2 p0 = { v0.pos.x, v0.pos.y };
		int2 p1 = { v1.pos.x, v1.pos.y };

		graphics.DrawLine(p0, p1, drawingColor);
	}
}

void Engine::DrawTriangleList(Vertex* vertices, unsigned int cVertices) {
	for (int i = 2; i < cVertices; i += 3) {
		Vertex& v0 = vertices[i - 2];
		Vertex& v1 = vertices[i - 1];
		Vertex& v2 = vertices[i];

		int2 p0 = { v0.pos.x, v0.pos.y };
		int2 p1 = { v1.pos.x, v1.pos.y };
		int2 p2 = { v2.pos.x, v2.pos.y };

		ColorBlock color = drawingColor;
		if (i < 6) color = Color::cyan;		// temporary: highlight anything on the front face to mark it and e.g. test HSD/z-buffering

		if (bWireframe) graphics.DrawTriangle(p0, p1, p2, color);
		else			graphics.FillTriangle(p0, p1, p2, color);
	}
}

void Engine::DrawTriangleStrip(Vertex* vertices, unsigned int cVertices) {
	for (int i = 2; i < cVertices; i++) {
		Vertex& v0 = vertices[i - 2];
		Vertex& v1 = vertices[i - 1];
		Vertex& v2 = vertices[i];

		int2 p0 = { v0.pos.x, v0.pos.y };
		int2 p1 = { v1.pos.x, v1.pos.y };
		int2 p2 = { v2.pos.x, v2.pos.y };

		ColorBlock color = drawingColor;
		if (i < 3) color = Color::cyan;		// temporary

		if (bWireframe) graphics.DrawTriangle(p0, p1, p2, color);
		else			graphics.FillTriangle(p0, p1, p2, color);
	}
}

// To do:
// --- Use project13 text rendering
// --- Implement temporary buttons and sliders to facilitate fine-tuning and tweaking some values like
// FOV, z_offset, scroll mode, wireframe switch and transformations
// 
// --- Implement a near and far clipping plane and/or
// change the perspective equations so that points would collapse at the center after reaching some distance
// rather than allowing those points to go past that center point and appear as zoomed-in again

// --- Inspect the formulas in ToWorld and make sure they get exact with x and y
 
// --- Fix transformation pipeline order
// Geometry struct needs new methods like PerspectiveTransformation and PerspectiveTransformationReverse, AspectTransformation, ..., and so on
// Then these methods should be called at the right moment in the pipeline, either by ToScreen or somewhere else
// 
// Otherwise the cube is going to be rendered malformed because of e.g. perspective variables being used in the wrong place

// --- Implement a camera class

// --- Implement z-buffering or any other hidden-surface/line-determination algorithm
// --- Base mesh generation on the .obj format and implement .obj format mesh loading

// --- Add a grid plane to the scene

// --- Performance consideration; implement buffer swapping / swap chain by using a pointer, instead of performing bit block transfers to the front buffer

// --- Implement z-buffering
// --- Implement culling after z-buffering, so that it doesn't get in the way

// --- Implement a flexible vertex format; e.g. color, normals, texture coordinates, etc. in the vertex mesh element structure

// --- Move from vectors to pointer arrays, especially in mesh vertex buffers etc.
// Copying vectors passed as function parameters will decrease performance, instead pass the pointer with array size and operate on data in the array directly

// To do: Implement an option to set one of two types of a vertex pipeline
// - a two-way vertex pipeline with reverse methods using twice the processing power
// - a one-way vertex pipeline without reverse methods using twice the memory to store the computed vertiex coordinates outside of the original buffer

