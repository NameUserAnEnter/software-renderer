#include "Engine.h"

float Geometry::FOV = 0.6f;
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

	InitCustomScene();
}

void Engine::Release() {
	scene.End();
	graphics.ReleaseBuffers();
}

void Engine::Update() {
	// Clear backbuffer
	graphics.ClearBackBuffer();

	// Render geometry
	RenderScene();

	// Swap buffers
	graphics.UpdateFrontBuffer();
}

void Engine::OnKeyPress() {
}

void Engine::OnKeyRelease() {
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

	point3 v111 = { -1,  1, -1 };
	point3 v211 = { 1,  1, -1 };
	point3 v121 = { -1, -1, -1 };
	point3 v221 = { 1, -1, -1 };

	point3 v112 = { -1,  1, 1 };
	point3 v212 = { 1,  1, 1 };
	point3 v122 = { -1, -1, 1 };
	point3 v222 = { 1, -1, 1 };

	// Front face
	cube.AddVertex(v111);
	cube.AddVertex(v211);
	cube.AddVertex(v221);
	//cube.AddVertex(v111);

	cube.AddVertex(v111);
	cube.AddVertex(v221);
	cube.AddVertex(v121);
	//cube.AddVertex(v111);

	// Left face
	cube.AddVertex(v111);
	cube.AddVertex(v121);
	cube.AddVertex(v112);
	//cube.AddVertex(v111);

	cube.AddVertex(v111);
	cube.AddVertex(v122);
	cube.AddVertex(v112);
	//cube.AddVertex(v111);

	// Back face
	cube.AddVertex(v112);
	cube.AddVertex(v122);
	cube.AddVertex(v212);
	//cube.AddVertex(v112);

	cube.AddVertex(v112);
	cube.AddVertex(v222);
	cube.AddVertex(v212);
	//cube.AddVertex(v112);

	// Right face
	cube.AddVertex(v212);
	cube.AddVertex(v222);
	cube.AddVertex(v211);
	//cube.AddVertex(v212);

	cube.AddVertex(v211);
	cube.AddVertex(v222);
	cube.AddVertex(v221);
	//cube.AddVertex(v211);

	// Top face
	cube.AddVertex(v211);
	cube.AddVertex(v111);
	cube.AddVertex(v212);
	//cube.AddVertex(v211);

	cube.AddVertex(v211);
	cube.AddVertex(v111);
	cube.AddVertex(v112);
	//cube.AddVertex(v211);

	// Bottom face
	cube.AddVertex(v221);
	cube.AddVertex(v122);
	cube.AddVertex(v121);
	//cube.AddVertex(v221);

	cube.AddVertex(v221);
	cube.AddVertex(v122);
	cube.AddVertex(v222);
	//cube.AddVertex(v221);

	cube.scale = 0.5;
	cube.pos = { 0.0, 0.0, 0.0 };

	scene.Begin();
	scene.AddMesh(cube);
}

void Engine::RenderScene() {
	for (Mesh mesh : scene.meshList) {
		std::string output = "";

		for (int i = 1; i < mesh.vertices.size(); i++) {
			point3 v1 = mesh.vertices[i - 1];
			point3 v2 = mesh.vertices[i];

			v1 = Geometry::Scale(v1, mesh.scale);
			v2 = Geometry::Scale(v2, mesh.scale);

			v1 = Geometry::Translate(v1, mesh.pos);
			v2 = Geometry::Translate(v2, mesh.pos);

			point2 p1 = Geometry::ToScreen(v1);
			point2 p2 = Geometry::ToScreen(v2);

			graphics.DrawLine(p1.x, p1.y, p2.x, p2.y, wireframeColor);

			if (i == 1 || i == 3) {
				output += std::to_string(v1.x) + ", " + std::to_string(v1.y) + " -> ";
				output += std::to_string(p1.x) + ", " + std::to_string(p1.y) + " || ";
			}
		}

		SetTitle(hWindow, output);
	}
}

