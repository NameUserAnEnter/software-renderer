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
	wcex.hbrBackground = CreateSolidBrush(RGB(10, 0, 30));

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
	unsigned int x = 20, y = 20, width = 100, height = 50;
	ColorBlockTransparent color = { 170, 0, 255, 255 };

	bool bDone = false;
	while (!bDone) {
		// Clear backbuffer
		graphics.ClearBackBuffer();

		// Render geometry
		// ...

		// Swap buffers
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

	graphics.ReleaseBuffers();
	return 0;
}

