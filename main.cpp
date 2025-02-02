#include <windows.h>
#include "Graphics.h"
#include "Shape.h"
#include "Scene.h"
#include "Geometry.h"
#include "utils.h"

float Geometry::FOV = 0.6f;
unsigned int Geometry::uViewportWidth = 800;
unsigned int Geometry::uViewportHeight = 600;

LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	HWND hWnd;
	HBRUSH hbrBackground = CreateSolidBrush(RGB(10, 0, 30));
	unsigned int uWindowWidth = Geometry::uViewportWidth;
	unsigned int uWindowHeight = Geometry::uViewportHeight;
	wchar_t szWindowClass[] = L"window class";

	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.lpfnWndProc = WndProc;

	wcex.hInstance = hInstance;
	wcex.lpszClassName = szWindowClass;
	wcex.hbrBackground = hbrBackground;

	RegisterClassEx(&wcex);
	hWnd = CreateWindow(szWindowClass, L"Main", WS_OVERLAPPEDWINDOW, 0, 0, uWindowWidth, uWindowHeight, NULL, NULL, hInstance, NULL);
	if (hWnd == NULL) {
		MessageBox(NULL, L"Cannot create application window.", L"Error", MB_OK);
		return 1;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	RECT ClientRect;
	GetClientRect(hWnd, &ClientRect);

	unsigned int uClientWidth = ClientRect.right;
	unsigned int uClientHeight = ClientRect.bottom;

	Geometry::uViewportWidth = uClientWidth;
	Geometry::uViewportHeight = uClientHeight;

	MSG msg;

	Graphics graphics(hWnd); //, uWindowWidth, uWindowHeight);
	graphics.ResizeBuffers(uClientWidth, uClientHeight);		// also calls InitializeBuffers()
	ColorBlockTransparent color = { 170, 0, 255, 255 };

	Scene scene;
	scene.Begin();

	scene.AddMesh(Mesh());
	scene.meshList.back().AddVertex(-1.0, -1.0, -1.0);
	scene.meshList.back().AddVertex( 1.0, -1.0, -1.0);
	scene.meshList.back().AddVertex( 1.0,  1.0, -1.0);
	scene.meshList.back().AddVertex(-1.0,  1.0, -1.0);

	scene.meshList.back().AddVertex(-1.0, -1.0, 1.0);
	scene.meshList.back().AddVertex(1.0, -1.0, 1.0);
	scene.meshList.back().AddVertex(1.0, 1.0, 1.0);
	scene.meshList.back().AddVertex(-1.0, 1.0, 1.0);

	scene.meshList.back().Scale(0.3);

	bool bDone = false;
	while (!bDone) {
		// Clear backbuffer
		graphics.ClearBackBuffer();

		// Render geometry
		for (Mesh mesh : scene.meshList) {
			std::string output = "";

			for (int i = 1; i < mesh.vertices.size(); i++) {
				point3 v1 = mesh.vertices[i - 1];
				point3 v2 = mesh.vertices[i];

				point2 p1 = Geometry::ToScreen(v1);
				point2 p2 = Geometry::ToScreen(v2);

				graphics.DrawLine(p1.x, p1.y, p2.x, p2.y, color);

				if (i == 1 || i == 3) {
					output += std::to_string(v1.x) + ", " + std::to_string(v1.y) + " -> ";
					output += std::to_string(p1.x) + ", " + std::to_string(p1.y) + " || ";
				}
			}

			SetTitle(hWnd, output);
		}

		// Swap buffers
		graphics.UpdateFrontBuffer();

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) {
			switch (msg.message) {
				case WM_KEYDOWN:
					break;
				case WM_SIZE:
					graphics.ResizeBuffers(LOWORD(msg.lParam), HIWORD(msg.lParam));

					Geometry::uViewportWidth = LOWORD(msg.lParam);
					Geometry::uViewportHeight = HIWORD(msg.lParam);
					break;
				case WM_QUIT:
					bDone = true;
					break;
				default:
					TranslateMessage(&msg);
					DispatchMessage(&msg);
			}
		}
	}

	scene.End();
	graphics.ReleaseBuffers();
	return 0;
}

