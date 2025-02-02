#include <windows.h>
#include "Engine.h";

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

	MSG msg;

	Engine engine;
	engine.Init(hWnd);

	bool bDone = false;
	while (!bDone) {
		engine.Update();

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) {
			switch (msg.message) {
				case WM_KEYUP:
					engine.OnKeyRelease();
					break;
				case WM_KEYDOWN:
					engine.OnKeyPress();
					break;
				case WM_SIZE:
					engine.OnWindowResize(LOWORD(msg.lParam), HIWORD(msg.lParam));
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

	engine.Release();
	return 0;
}

