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

	textOffset = { 20, 20 };

	uLineThickness = 1;

	bWireframe = true;
	bRenderBuffered = true;

	bUseTestColors = true;

	testColors.push_back({ 255,   0, 255 });
	testColors.push_back({   0, 127, 255 });
	testColors.push_back({ 127,   0, 255 });
	testColors.push_back({   0, 255, 255 });
	testColors.push_back({ 255,   0, 127 });
	testColors.push_back({   0, 255, 127 });
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
	graphics.SetRasterUnitThickness(uLineThickness);

	scene.Begin();
	InitCustomScene();
}

void Engine::Release() {
	scene.End();
	graphics.ReleaseBuffers();
}

void Engine::Update() {
	output += "viewport resolution: " + NumStr(viewport.viewportSize.x) + "x" + NumStr(viewport.viewportSize.y) + "\n\n";

	// Logic
	ReadUserInput();

	Clock::TimeUpdate();
	Input::UpdateInputs();	// resets some inputs to a non-activated state, needs to be called after the input is read

	// Clear backbuffer
	graphics.ClearBackBuffer();

	static bool fillShape = false;
	Quad q = {
		{ viewport.viewportSize.x / 2 - 400, viewport.viewportSize.y / 2 - 200 },
		{ viewport.viewportSize.x / 2 + 300, viewport.viewportSize.y / 2 - 100 },
		{ viewport.viewportSize.x / 2 + 400, viewport.viewportSize.y / 2 + 200 },
		{ viewport.viewportSize.x / 2 - 300, viewport.viewportSize.y / 2 + 100 },
	};

	static auto r = Quad::BoundingBox(q);
	static const float offsetVal = 0;
	static bool bOffset = true;
	if (bOffset) {
		r.p1.x -= offsetVal;
		r.p1.y -= offsetVal;
		r.p2.x += offsetVal;
		r.p2.y -= offsetVal;
		r.p3.x += offsetVal;
		r.p3.y += offsetVal;
		r.p4.x -= offsetVal;
		r.p4.y += offsetVal;

		//r.p2.x = 1062;
		//r.p2.y = 260;
		bOffset = false;
	}

	int2 m = { Input::Mouse_x, Input::Mouse_y };

	if (Input::Mouse[LMB]) {
		float d1 = sqrt(pow(m.x - r.p1.x, 2) + pow(m.y - r.p1.y, 2));
		float d2 = sqrt(pow(m.x - r.p2.x, 2) + pow(m.y - r.p2.y, 2));
		float d3 = sqrt(pow(m.x - r.p3.x, 2) + pow(m.y - r.p3.y, 2));
		float d4 = sqrt(pow(m.x - r.p4.x, 2) + pow(m.y - r.p4.y, 2));

		float dmin = fmin(d1, d2); dmin = fmin(dmin, d3); dmin = fmin(dmin, d4);

		if (dmin == d1) r.p1 = m;
		else if (dmin == d2) r.p2 = m;
		else if (dmin == d3) r.p3 = m;
		else if (dmin == d4) r.p4 = m;
	}

	//if (Input::Mouse[LMB]) {
	//	if (q.PointWithin(m)) fillShape = true;
	//	else fillShape = false;
	//}

	Print(NumStr(Input::Mouse[LMB]));

	graphics.DrawQuad(r.p1, r.p2, r.p3, r.p4, Color::cyan);

	if (!((Quad) r).CoversOther(q)) {
		if (!fillShape)	graphics.DrawQuad(q.p1, q.p2, q.p3, q.p4, Color::cyan);
		else			graphics.FillQuad(q.p1, q.p2, q.p3, q.p4, Color::cyan);
	}

	graphics.Print(NumStr(r.p1.x) + ", " + NumStr(r.p1.y), r.p1.x, r.p1.y);
	graphics.Print(NumStr(r.p2.x) + ", " + NumStr(r.p2.y), r.p2.x, r.p2.y);
	graphics.Print(NumStr(r.p3.x) + ", " + NumStr(r.p3.y), r.p3.x, r.p3.y);
	graphics.Print(NumStr(r.p4.x) + ", " + NumStr(r.p4.y), r.p4.x, r.p4.y);

	graphics.Print(NumStr(q.p1.x) + ", " + NumStr(q.p1.y), q.p1.x, q.p1.y);
	graphics.Print(NumStr(q.p2.x) + ", " + NumStr(q.p2.y), q.p2.x, q.p2.y);
	graphics.Print(NumStr(q.p3.x) + ", " + NumStr(q.p3.y), q.p3.x, q.p3.y);
	graphics.Print(NumStr(q.p4.x) + ", " + NumStr(q.p4.y), q.p4.x, q.p4.y);

	// Render scene geometry
	//RenderScene();

	// Print info
	//UpdateOutput();

	// Present the scene
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
	Print("Initializing scene...", true, ALIGN_CENTER);

	//InitCustomModels();

	InitModelCube();
	//InitModelCar();

	Mesh& mesh = *scene.meshes[scene.GetMeshCount() - 1];

	saved_pos	= mesh.t.pos;
	saved_angle = mesh.t.angle;

	if (!bUseTestColors || testColors.size() < 2) return;

	for (int i = 3; i < mesh.GetVertexCount(); i += 4) {
		int r = ((i - 3) / 4) % testColors.size();

		for (int j = 0; j < testColors.size(); j++) {
			if (r == j) mesh.vertices[i - 3].color = testColors[r];
		}
	}
}

void Engine::InitCustomModels() {
	scene.AddMesh();

	if (scene.GetMeshCount() <= 0) return;
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

	cube.topology = QUAD_LIST;

	if (cube.topology == POINT_LIST) {
		cube.AddVertex(v111);
		cube.AddVertex(v211);
		cube.AddVertex(v121);
		cube.AddVertex(v221);
		cube.AddVertex(v112);
		cube.AddVertex(v212);
		cube.AddVertex(v122);
		cube.AddVertex(v222);
	}
	else if (cube.topology == LINE_LIST) {
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
	else if (cube.topology == LINE_STRIP) {
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
	else if (cube.topology == TRIANGLE_LIST) {
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
	else if (cube.topology == TRIANGLE_STRIP) {
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
	else if (cube.topology == QUAD_LIST) {
		//v 1.000000 1.000000 -1.000000
		//v 1.000000 -1.000000 -1.000000
		//v 1.000000 1.000000 1.000000
		//v 1.000000 -1.000000 1.000000
		//v -1.000000 1.000000 -1.000000
		//v -1.000000 -1.000000 -1.000000
		//v -1.000000 1.000000 1.000000
		//v -1.000000 -1.000000 1.000000
		//s off
		//f 1 5 7 3
		//f 4 3 7 8
		//f 8 7 5 6
		//f 6 2 4 8
		//f 2 1 3 4
		//f 6 5 1 2

		Vertex v[] = {
			{  1,  1, -1 },
			{  1, -1, -1 },
			{  1,  1,  1 },
			{  1, -1,  1 },
			{ -1,  1, -1 },
			{ -1, -1, -1 },
			{ -1,  1,  1 },
			{ -1, -1,  1 }
		};

		cube.AddVertex(v[1 - 1]); cube.AddVertex(v[5 - 1]); cube.AddVertex(v[7 - 1]); cube.AddVertex(v[3 - 1]);
		cube.AddVertex(v[4 - 1]); cube.AddVertex(v[3 - 1]); cube.AddVertex(v[7 - 1]); cube.AddVertex(v[8 - 1]);
		cube.AddVertex(v[8 - 1]); cube.AddVertex(v[7 - 1]); cube.AddVertex(v[5 - 1]); cube.AddVertex(v[6 - 1]);
		cube.AddVertex(v[6 - 1]); cube.AddVertex(v[2 - 1]); cube.AddVertex(v[4 - 1]); cube.AddVertex(v[8 - 1]);
		cube.AddVertex(v[2 - 1]); cube.AddVertex(v[1 - 1]); cube.AddVertex(v[3 - 1]); cube.AddVertex(v[4 - 1]);
		cube.AddVertex(v[6 - 1]); cube.AddVertex(v[5 - 1]); cube.AddVertex(v[1 - 1]); cube.AddVertex(v[2 - 1]);
	}
}

void Engine::InitModelCube() {
	scene.AddMesh("data/cube4.obj");

	if (scene.GetMeshCount() <= 0) return;
	Mesh& mesh = *scene.meshes[scene.GetMeshCount() - 1];

	mesh.t.scale = { 0.6, 0.6, 0.6 };
	mesh.t.angle = { PI / -10, 0, 0 };
	//mesh.t.angle = { PI / -10, PI, 0 };
}

void Engine::InitModelCar() {
	scene.AddMesh("data/car1.obj");

	if (scene.GetMeshCount() <= 0) return;
	Mesh& mesh = *scene.meshes[scene.GetMeshCount() - 1];

	mesh.t.scale = { 0.6, 0.6, 0.6 };
	mesh.t.pos = { 0, -0.6, 0 };
	mesh.t.angle = { 0, PI / -2.4, 0 };
}

void Engine::ReadUserInput() {
	if (Input::Esc) StopEngine();

	if (scene.GetMeshCount() <= 0) return;
	Mesh& mesh = *scene.meshes[scene.GetMeshCount() - 1];
	Transformation& controlled = mesh.t;

	float delta_pos = 0.01;
	float delta_scroll = 0.01;
	float delta_angle = (PI / 4) * 0.001;

	if (Input::Shift) delta_angle *= 15;
	if (Input::Shift) delta_pos *= 5;

	if (Input::Alpha[Q]) controlled.angle.y += delta_angle;
	if (Input::Alpha[E]) controlled.angle.y -= delta_angle;

	if (Input::Arrow[UP]) controlled.angle.x += delta_angle;
	if (Input::Arrow[DOWN]) controlled.angle.x -= delta_angle;

	if (Input::Arrow[LEFT]) controlled.pos.z += delta_pos;
	if (Input::Arrow[RIGHT]) controlled.pos.z -= delta_pos;

	if (Input::Alpha[W]) controlled.pos.y += delta_pos;
	if (Input::Alpha[S]) controlled.pos.y -= delta_pos;

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

	output += "scroll mode: " + NumStr(scroll_mode) + "\n\n";

	if (Input::Shift) delta_scroll *= 10;

	if (Input::Mouse[mouse_control::SCROLL_DOWN])	*scrollable_values[scroll_mode] -= delta_scroll;
	if (Input::Mouse[mouse_control::SCROLL_UP])		*scrollable_values[scroll_mode] += delta_scroll;

	controlled.scale = { scale, scale, scale };
}

void Engine::UpdateOutput() {
	if (scene.GetMeshCount() <= 0) return;
	Mesh& mesh = *scene.meshes[scene.GetMeshCount() - 1];

	output += "mesh pos:    ("	+ NumStr(mesh.t.pos.x) + ", " + NumStr(mesh.t.pos.y) + ", " + NumStr(mesh.t.pos.z) + ")\n";
	output += "mesh angle:  ("	+ NumStr(mesh.t.angle.x) + ", " + NumStr(mesh.t.angle.y) + ", " + NumStr(mesh.t.angle.z) + ")\n";
	output += "mesh scale:  ("	+ NumStr(mesh.t.scale.x) + ", " + NumStr(mesh.t.scale.y) + ", " + NumStr(mesh.t.scale.z) + ")\n\n";

	output += "z_offset:    "	+ NumStr(viewport.z_offset) + "\n";
	output += "FOV:         "	+ NumStr(viewport.FOV);
	
	if (mesh.GetVertexCount() > 0) {
		float3 p = mesh.vertices[0].pos;
		output += "\n\nvertex0: ("	+ NumStr(p.x) + ", " + NumStr(p.y) + ", " + NumStr(p.z) + ")\n";
	}

	output += "\nFramerate: " + NumStr(Clock::updates_in_last_sec);

	Print(output);
	output = "";
}

void Engine::Print(std::string str, bool updateFrontBuffer, TEXT_ORIENTATION orientation) {
	Print(toWide(str), updateFrontBuffer, orientation);
}

void Engine::Print(std::wstring str, bool updateFrontBuffer, TEXT_ORIENTATION orientation) {
	if (updateFrontBuffer) graphics.ClearBackBuffer();

	graphics.Print(str, textOffset.x, textOffset.y, orientation);
	if (updateFrontBuffer) graphics.UpdateFrontBuffer();
}

void Engine::RenderScene() {
	for (int i = 0; i < scene.GetMeshCount(); i++) {
		Mesh& mesh = *scene.meshes[i];

		Vertex* vertices;
		if (bRenderBuffered) {
			mesh.ApplyTransformationOnOutputBuffer();
			vertices = mesh.outputBuffer;
		}
		else {
			// applying transformation on mesh is currently non-reversible, so non-buffered rendering is left in code only to test potential performance gain
			//mesh.ApplyTransformation();
			vertices = mesh.vertices;
		}

		unsigned int cVertices = mesh.GetVertexCount();
		VerticesToScreen(vertices, cVertices);

		switch (mesh.topology) {
			case POINT_LIST:		DrawPointList(vertices, cVertices);		break;
			case LINE_LIST:			DrawLineList(vertices, cVertices);		break;
			case LINE_STRIP:		DrawLineStrip(vertices, cVertices);		break;
			case TRIANGLE_LIST:		DrawTriangleList(vertices, cVertices);	break;
			case TRIANGLE_STRIP:	DrawTriangleStrip(vertices, cVertices);	break;
			case QUAD_LIST:			DrawQuadList(vertices, cVertices);		break;
		}

		if (!bRenderBuffered) VerticesToScreenReverse(vertices, cVertices);
	}
}

void Engine::VerticesToScreen(Vertex* vertices, unsigned int cVertices) {
	for (int i = 0; i < cVertices; i++) {
		viewport.VertexAspectTransformation(vertices[i].pos);
		viewport.VertexPerspectiveTransformation(vertices[i].pos);
		viewport.VertexScreenTransformation(vertices[i].pos);
	}
}

void Engine::VerticesToScreenReverse(Vertex* vertices, unsigned int cVertices) {
	for (int i = 0; i < cVertices; i++) {
		viewport.VertexScreenTransformationReverse(vertices[i].pos);
		viewport.VertexPerspectiveTransformationReverse(vertices[i].pos);
		viewport.VertexAspectTransformationReverse(vertices[i].pos);
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
		if (i < 8) color = Color::cyan;		// temporary highlight for future z-buffer testing

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
		if (i < 4) color = Color::cyan;		// temporary highlight for future z-buffer testing

		if (bWireframe) graphics.DrawTriangle(p0, p1, p2, color);
		else			graphics.FillTriangle(p0, p1, p2, color);
	}
}

void Engine::DrawQuadList(Vertex* vertices, unsigned int cVertices) {
	// polygon z-buffering? attempt

	static bool popup = true;
	std::string output;

	float fz;
	if (cVertices >= 3) fz = vertices[3].pos.z;		// initial value

	for (int i = 3; i < cVertices; i += 4) {
		const Vertex& v0 = vertices[i - 3];
		const Vertex& v1 = vertices[i - 2];
		const Vertex& v2 = vertices[i - 1];
		const Vertex& v3 = vertices[i];

		// face-z - furthest from the camera z from all face vertices
		float maxz = fmax(v0.pos.z, v1.pos.z); maxz = fmax(maxz, v2.pos.z); maxz = fmax(maxz, v3.pos.z);

		if (maxz > fz) {
			continue;
		}
		fz = maxz;

		if (popup) {
			output.clear();

			output += "fz: " + NumStr(fz) + "\n";
			output += "maxz: " + NumStr(maxz) + "\n";

			graphics.ClearBackBuffer();
		}

		int2 p0 = { v0.pos.x, v0.pos.y };
		int2 p1 = { v1.pos.x, v1.pos.y };
		int2 p2 = { v2.pos.x, v2.pos.y };
		int2 p3 = { v3.pos.x, v3.pos.y };

		if (bWireframe) graphics.DrawQuad(p0, p1, p2, p3, v0.color);
		else			graphics.FillQuad(p0, p1, p2, p3, v0.color);

		if (popup) {
			//Print(output);
			graphics.UpdateFrontBuffer();

			Popup(output);
			//Popup();
		}
	}

	popup = false;
}

// To do:
// --- Implement buttons and sliders to tweak some values like FOV, z_offset, scroll mode, wireframe switch and transformations
// --- Implement a near and far clipping plane and/or
// change the perspective equations so that points would collapse at the center after reaching some distance
// rather than allowing those points to go past that center point and appear flipped as zoomed-in again
// --- Implement a camera class
// --- Implement camera transformations
// --- Add a grid plane to the scene
// --- Performance consideration; implement buffer swapping / swap chain by using a pointer, instead of performing bit block transfers to the front buffer
// --- Implement z-buffering or optionally some other hidden surface/line determination algorithm like the painter's algorithm
// --- Implement culling after z-buffering, so that it doesn't get in the way
// --- Implement a flexible vertex format; e.g. color, normals, texture coordinates, etc. in the vertex mesh element structure
// --- Implement scene vertex, face, polygon counters
// --- Implement an option to draw polygons using indices to a point list like vertex buffer with no duplicate vertices, compare performance
// --- Implement ambient lighting
// --- Switch to vector and matrix based geometric calculations and implement affine transformations
// --- Implement multi-threading

