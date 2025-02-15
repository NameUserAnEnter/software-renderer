#include <windows.h>
#include "Engine.h";

LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Engine	engine;		// default constructor sets some variables to be used for CreateWindow(), hence object is created here already
	HWND	hWnd;

	wchar_t szWindowClass[] = L"window class";

	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.lpfnWndProc = WndProc;

	wcex.hInstance = hInstance;
	wcex.lpszClassName = szWindowClass;
	wcex.hbrBackground = CreateSolidBrush(RGB(engine.backgroundColor.R, engine.backgroundColor.G, engine.backgroundColor.B));

	RegisterClassEx(&wcex);

	hWnd = CreateWindow(szWindowClass,
		engine.windowTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		engine.windowX, engine.windowY,
		engine.uWindowWidth, engine.uWindowHeight,
		NULL, NULL, hInstance, NULL);

	if (!hWnd) {
		MessageBox(NULL, L"Cannot create application window.", L"Error", MB_OK);
		return 1;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	MSG msg;

	engine.Init(hWnd);
	while (!engine.Done()) {
		engine.Update();

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) {
			switch (msg.message) {
				case WM_SIZE:
					engine.OnWindowResize(LOWORD(msg.lParam), HIWORD(msg.lParam));
					break;
				case WM_QUIT:
					engine.StopEngine();
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

LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			Input::SetInputs(true, wParam, lParam);
			break;

		case WM_LBUTTONUP:
			Input::SetInputs(false, MK_LBUTTON, lParam);
			break;

		case WM_RBUTTONUP:
			Input::SetInputs(false, MK_RBUTTON, lParam);
			break;

		case WM_MBUTTONUP:
			Input::SetInputs(false, MK_MBUTTON, lParam);
			break;

		case WM_MOUSEWHEEL:
			Input::SetScroll(wParam, lParam);
			break;

		case WM_MOUSEMOVE:
			Input::SetMousePos(lParam);
			break;

		case WM_KEYUP:
			Input::SetInputs(false, wParam);
			break;
		case WM_KEYDOWN:
			Input::SetInputs(true, wParam);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

