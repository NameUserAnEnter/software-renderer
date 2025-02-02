#include <windows.h>
#include "Graphics.h"
#include "Shape.h"
#include "utils.h"

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
	unsigned int uWindowWidth = 800;
	unsigned int uWindowHeight = 600;
	wchar_t szWindowClass[] = L"window class";

	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.lpfnWndProc = WndProc;

	wcex.hInstance = hInstance;
	wcex.lpszClassName = szWindowClass;
	wcex.hbrBackground = CreateSolidBrush(RGB(30, 30, 30));

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

	MSG msg;

	Graphics graphics(hWnd); //, uWindowWidth, uWindowHeight);
	graphics.ResizeBuffers(ClientRect.right, ClientRect.bottom);		// also calls InitializeBuffers()

	// Temporary test shape initialization for test rendering
	unsigned int x = 20, y = 20, width = 50, height = 50;
	unsigned int lineThicknes = 5;
	ColorBlockTransparent color = { 70, 0, 255, 255 };

	//Shape shape;
	//shape.x = x;
	//shape.y = x;
	//shape.Resize(width, height);
	//shape.FillWithColor(color);

	unsigned int it = 0;

	bool bDone = false;
	while (!bDone) {
		graphics.ClearBackBuffer();

		it++;

		// Rendering
		// Test rendering
		//graphics.SetOnBackBuffer(shape.x, shape.y, shape.GetWidth(), shape.GetHeight(), shape.GetBytes());
		
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (i < lineThicknes || j < lineThicknes || i >= height - lineThicknes || j >= width - lineThicknes) {
					graphics.SetOnBackBuffer(x + j, y + i, color);
				}
			}
		}

		graphics.UpdateFrontBuffer();

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) {
			switch (msg.message) {
				case WM_SIZE:
					graphics.ResizeBuffers(LOWORD(msg.lParam), HIWORD(msg.lParam));
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

	//shape.ReleaseShape();
	graphics.ReleaseBuffers();
	return 0;
}

