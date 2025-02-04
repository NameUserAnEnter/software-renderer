#include "Engine.h"

float Geometry::z_offset = 1;
float Geometry::FOV = 0.06;
unsigned int Geometry::uViewportWidth = 1280;
unsigned int Geometry::uViewportHeight = 960;

void Engine::Init(HWND hWindow) {
	this->hWindow = hWindow;

	RECT ClientRect;
	GetClientRect(hWindow, &ClientRect);

	unsigned int uClientWidth = ClientRect.right;
	unsigned int uClientHeight = ClientRect.bottom;

	Geometry::uViewportWidth = uClientWidth;
	Geometry::uViewportHeight = uClientHeight;

	graphics.Init(hWindow);
	graphics.ResizeBuffers(uClientWidth, uClientHeight);		// also calls InitializeBuffers()
	wireframeColor = Color::white;

	breakOut = false;

	InitCustomScene();
}

void Engine::Release() {
	scene.End();
	graphics.ReleaseBuffers();
}

bool Engine::Update() {
	// Logic
	MoveObjects();
	ReadInputs();
	Input::UpdateInputs();

	// Clear backbuffer
	graphics.ClearBackBuffer();

	// Render geometry
	RenderScene();

	// Swap buffers
	graphics.UpdateFrontBuffer();

	return breakOut;
}

void Engine::OnWindowResize(unsigned int uNewClientWidth, unsigned int uNewClientHeight) {
	graphics.ResizeBuffers(uNewClientWidth, uNewClientHeight);

	Geometry::uViewportWidth = uNewClientWidth;
	Geometry::uViewportHeight = uNewClientHeight;
}

void Engine::InitCustomScene() {
	InitObjects();

	Mesh& cube = scene.meshList.back();

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

void Engine::InitObjects() {
	Mesh cube;

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

	float3 v111 = { -1,  1, -1 };
	float3 v211 = {  1,  1, -1 };
	float3 v121 = { -1, -1, -1 };
	float3 v221 = {  1, -1, -1 };

	float3 v112 = { -1,  1, 1 };
	float3 v212 = {  1,  1, 1 };
	float3 v122 = { -1, -1, 1 };
	float3 v222 = {  1, -1, 1 };

	// Front face
	cube.AddVertex(v111);
	cube.AddVertex(v211);
	cube.AddVertex(v221);
	cube.AddVertex(v111);

	cube.AddVertex(v111);
	cube.AddVertex(v221);
	cube.AddVertex(v121);
	cube.AddVertex(v111);

	// Left face
	cube.AddVertex(v111);
	cube.AddVertex(v121);
	cube.AddVertex(v112);
	cube.AddVertex(v111);

	cube.AddVertex(v122);
	cube.AddVertex(v121);
	cube.AddVertex(v112);
	cube.AddVertex(v122);

	// Back face
	cube.AddVertex(v112);
	cube.AddVertex(v212);
	cube.AddVertex(v122);
	cube.AddVertex(v112);

	cube.AddVertex(v222);
	cube.AddVertex(v122);
	cube.AddVertex(v212);
	cube.AddVertex(v222);

	// Right face
	cube.AddVertex(v212);
	cube.AddVertex(v222);
	cube.AddVertex(v211);
	cube.AddVertex(v212);

	cube.AddVertex(v211);
	cube.AddVertex(v222);
	cube.AddVertex(v221);
	cube.AddVertex(v211);

	// Top face
	cube.AddVertex(v111);
	cube.AddVertex(v112);
	cube.AddVertex(v211);
	cube.AddVertex(v111);

	cube.AddVertex(v112);
	cube.AddVertex(v212);
	cube.AddVertex(v211);
	cube.AddVertex(v112);

	// Bottom face
	cube.AddVertex(v121);
	cube.AddVertex(v122);
	cube.AddVertex(v222);
	cube.AddVertex(v121);

	cube.AddVertex(v222);
	cube.AddVertex(v221);
	cube.AddVertex(v121);
	cube.AddVertex(v222);

	scene.Begin();
	scene.AddMesh(cube);
}

void Engine::MoveObjects() {
}

void Engine::ReadInputs() {
	if (Input::Esc) breakOut = true;

	Mesh* controlled_mesh = &scene.meshList.back();

	float delta_pos = 0.01;
	float delta_scroll = 0.01;
	float delta_angle = (PI / 4) * 0.01;

	if (Input::Shift) delta_angle *= 5;

	if (Input::Alpha[Q]) controlled_mesh->angle.y -= delta_angle;
	if (Input::Alpha[E]) controlled_mesh->angle.y += delta_angle;

	if (Input::Alpha[W]) controlled_mesh->pos.z += delta_pos;
	if (Input::Alpha[S]) controlled_mesh->pos.z -= delta_pos;

	if (Input::Alpha[A]) controlled_mesh->pos.x -= delta_pos;
	if (Input::Alpha[D]) controlled_mesh->pos.x += delta_pos;

	if (Input::Alpha[R]) {
		controlled_mesh->pos = saved_pos;
		controlled_mesh->angle = saved_angle;
	}

	// To do: implement sliders to change those values and/or buttons to change scroll mode
	std::vector<float*> scrollable_values;
	scrollable_values.push_back(&controlled_mesh->scale);
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

void Engine::RenderScene() {
	for (Mesh mesh : scene.meshList) {
		for (int i = 1; i < mesh.vertices.size(); i++) {
			if (i % 4 == 0) continue;

			float3 v1 = mesh.vertices[i - 1];
			float3 v2 = mesh.vertices[i];

			// To do:
			// Fix transformation pipeline order
			// Geometry struct needs new methods like PerspectiveTransformation and PerspectiveTransformationReverse, AspectTransformation, ..., and so on
			// Then these methods should be called at the right moment in the pipeline, either by ToScreen or somewhere else
			// 
			// Otherwise the cube is going to be rendered malformed because of e.g. perspective variables being used in the wrong place
			//
			// To do: implement camera position, rotation and turn

			v1 = Geometry::RotateAroundAxisY(v1, mesh.angle.y);
			v2 = Geometry::RotateAroundAxisY(v2, mesh.angle.y);

			v1 = Geometry::RotateAroundAxisX(v1, mesh.angle.x);
			v2 = Geometry::RotateAroundAxisX(v2, mesh.angle.x);

			v1 = Geometry::RotateAroundAxisZ(v1, mesh.angle.z);
			v2 = Geometry::RotateAroundAxisZ(v2, mesh.angle.z);

			v1 = Geometry::Scale(v1, mesh.scale);
			v2 = Geometry::Scale(v2, mesh.scale);

			v1 = Geometry::Translate(v1, mesh.pos);
			v2 = Geometry::Translate(v2, mesh.pos);

			float2 p1 = Geometry::ToScreen(v1);
			float2 p2 = Geometry::ToScreen(v2);

			ColorBlockTransparent color = wireframeColor;
			
			if (i < 8) color = Color::cyan;		// highlight the front face
			graphics.DrawLine(p1.x, p1.y, p2.x, p2.y, color);
		}

		//output.clear();
		output += "mesh pos: ("		+ NumStr(mesh.pos.x) + ", " + NumStr(mesh.pos.y) + ", " + NumStr(mesh.pos.z) + "), ";
		output += "mesh scale: "	+ NumStr(mesh.scale) + ", ";
		output += "z_offset: "		+ NumStr(Geometry::z_offset) + ", ";
		output += "FOV: "			+ NumStr(Geometry::FOV);

		// To do: Use project13 text rendering
		SetWindowTitle(hWindow, output);
	}

	graphics.DrawRectangle(20, 20, 140, 70, Color::cyan);
	graphics.DrawTriangle(20, 160, 140, 200, 20, 100, Color::cyan);
	graphics.DrawQuad(20, 220, 100, 320, 70, 520, 150, 550, Color::cyan);
}

