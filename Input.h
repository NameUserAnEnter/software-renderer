#ifndef INPUT_H
#define INPUT_H

#include <windows.h>

enum alphabet
{
	A, B, C,
	D, E, F,
	G, H, I,
	J, K, L,
	M, N, O,
	P, Q, R, S,
	T, U, V,
	W, X, Y, Z
};

enum mouse_control
{
	LMB, MMB, RMB, SCROLL_UP, SCROLL_DOWN, MOVE
};

enum arrow
{
	UP, DOWN, RIGHT, LEFT
};

struct Input
{
	static bool Alpha[26];
	static bool Numeric[10];

	static bool Esc;
	static bool Enter;
	static bool Space;
	
	static bool Shift;
	static bool Ctrl;
	static bool Alt;
	static bool Tab;

	static bool Backspace;
	static bool Delete;

	static bool Mouse[6];
	static short Mouse_x;
	static short Mouse_y;

	static bool Arrow[4];

	static unsigned int Sub_mouse_x;
	static unsigned int Sub_mouse_y;

	static void SetInputs(bool, unsigned long long);
	static void SetInputs(bool, unsigned long long, long long);
	static void SetScroll(unsigned long long, long long);
	static void SetMousePos(long long);
	static void ReleaseMouseControls();
	static void UpdateInputs();
};

#endif

