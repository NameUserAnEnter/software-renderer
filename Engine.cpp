#include "Engine.h"

float Geometry::FOV = 0.6;
unsigned int Geometry::uViewportWidth = 800;
unsigned int Geometry::uViewportHeight = 600;

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
	wireframeColor = { 170, 0, 255, 255 };

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

	// Clear backbuffer
	graphics.ClearBackBuffer();

	// Render geometry
	RenderScene();

	// Swap buffers
	graphics.UpdateFrontBuffer();

	return breakOut;
}

void Engine::OnKeystateChange(bool pressed, unsigned long long code) {
	Input::SetInputs(pressed, code);
}

void Engine::OnWindowResize(unsigned int uNewClientWidth, unsigned int uNewClientHeight) {
	graphics.ResizeBuffers(uNewClientWidth, uNewClientHeight);

	Geometry::uViewportWidth = uNewClientWidth;
	Geometry::uViewportHeight = uNewClientHeight;
}

void Engine::InitCustomScene() {

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

	float3 v111 = { -1,  1, 1 };
	float3 v211 = { 1,  1, 1 };
	float3 v121 = { -1, -1, 1 };
	float3 v221 = { 1, -1, 1 };

	float3 v112 = { -1,  1, -1 };
	float3 v212 = { 1,  1, -1 };
	float3 v122 = { -1, -1, -1 };
	float3 v222 = { 1, -1, -1 };

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

	cube.scale = 1;
	cube.pos = { 0.0, 0.0, 0.0 };
	cube.angle = { 0.0, 0, 0.0 };

	scene.Begin();
	scene.AddMesh(cube);
}

void Engine::MoveObjects() {
}

void Engine::ReadInputs() {
	Mesh* controlled = &scene.meshList.back();
	static float delta = 0.01;
	static float delta_angle = (PI / 4) * 0.02;

	if (Input::Esc) breakOut = true;

	//if (Input::Alpha[W]) controlled->pos.z += delta;
	//if (Input::Alpha[S]) controlled->pos.z -= delta;

	if (Input::Alpha[Q]) controlled->angle.y -= delta_angle;
	if (Input::Alpha[E]) controlled->angle.y += delta_angle;

	if (Input::Alpha[W]) controlled->angle.x -= delta_angle;
	if (Input::Alpha[S]) controlled->angle.x += delta_angle;

	if (Input::Alpha[A]) controlled->angle.z -= delta_angle;
	if (Input::Alpha[D]) controlled->angle.z += delta_angle;
}

void Engine::RenderScene() {
	for (Mesh mesh : scene.meshList) {
		for (int i = 1; i < mesh.vertices.size(); i++) {
			if (i % 4 == 0) continue;

			float3 v1 = mesh.vertices[i - 1];
			float3 v2 = mesh.vertices[i];

			v1 = Geometry::RotateAroundAxisX(v1, mesh.angle.x);
			v2 = Geometry::RotateAroundAxisX(v2, mesh.angle.x);

			v1 = Geometry::RotateAroundAxisY(v1, mesh.angle.y);
			v2 = Geometry::RotateAroundAxisY(v2, mesh.angle.y);

			v1 = Geometry::RotateAroundAxisZ(v1, mesh.angle.z);
			v2 = Geometry::RotateAroundAxisZ(v2, mesh.angle.z);

			v1 = Geometry::Scale(v1, mesh.scale);
			v2 = Geometry::Scale(v2, mesh.scale);

			v1 = Geometry::Translate(v1, mesh.pos);
			v2 = Geometry::Translate(v2, mesh.pos);

			float2 p1 = Geometry::ToScreen(v1);
			float2 p2 = Geometry::ToScreen(v2);

			graphics.DrawLine(p1.x, p1.y, p2.x, p2.y, wireframeColor);
		}

		SetWindowTitle(hWindow, "scale: " + std::to_string(mesh.scale) + ", pos.z: " + std::to_string(mesh.pos.z) + ", FOV: " + std::to_string(Geometry::FOV));
	}
}

