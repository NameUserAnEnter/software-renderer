#include "Input.h"

bool Input::Alpha[26] = { };
bool Input::Numeric[10] = { };

bool Input::Esc = false;
bool Input::Enter = false;
bool Input::Space = false;

bool Input::Shift = false;
bool Input::Ctrl = false;
bool Input::Alt = false;
bool Input::Tab = false;

bool Input::Backspace = false;
bool Input::Delete = false;

bool Input::Mouse[6] = { };
short Input::Mouse_x = 0;
short Input::Mouse_y = 0;

bool Input::Arrow[4] = { };

unsigned int Input::Sub_mouse_x = 0;
unsigned int Input::Sub_mouse_y = 0;

void Input::SetInputs(bool state, unsigned long long input_code)
{
	if (input_code == VK_ESCAPE)		Esc = state;
	else if (input_code == VK_RETURN)	Enter = state;
	else if (input_code == VK_SPACE)	Space = state;
	else if (input_code >= 0x30 && input_code <= 0x39)
	{
		if		(input_code == 0x30)	Numeric[0] = state;
		else if (input_code == 0x31)	Numeric[1] = state;
		else if (input_code == 0x32)	Numeric[2] = state;
		else if (input_code == 0x33)	Numeric[3] = state;
		else if (input_code == 0x34)	Numeric[4] = state;
		else if (input_code == 0x35)	Numeric[5] = state;
		else if (input_code == 0x36)	Numeric[6] = state;
		else if (input_code == 0x37)	Numeric[7] = state;
		else if (input_code == 0x38)	Numeric[8] = state;
		else if (input_code == 0x39)	Numeric[9] = state;
	}
	else if (input_code >= 0x41 && input_code <= 0x5A)
	{
		if		(input_code == 0x41)	Alpha[alphabet::A] = state;
		else if (input_code == 0x42)	Alpha[alphabet::B] = state;
		else if (input_code == 0x43)	Alpha[alphabet::C] = state;
		else if (input_code == 0x44)	Alpha[alphabet::D] = state;
		else if (input_code == 0x45)	Alpha[alphabet::E] = state;
		else if (input_code == 0x46)	Alpha[alphabet::F] = state;
		else if (input_code == 0x47)	Alpha[alphabet::G] = state;
		else if (input_code == 0x48)	Alpha[alphabet::H] = state;
		else if (input_code == 0x49)	Alpha[alphabet::I] = state;
		else if (input_code == 0x4A)	Alpha[alphabet::J] = state;
		else if (input_code == 0x4B)	Alpha[alphabet::K] = state;
		else if (input_code == 0x4C)	Alpha[alphabet::L] = state;
		else if (input_code == 0x4D)	Alpha[alphabet::M] = state;
		else if (input_code == 0x4E)	Alpha[alphabet::N] = state;
		else if (input_code == 0x4F)	Alpha[alphabet::O] = state;
		else if (input_code == 0x50)	Alpha[alphabet::P] = state;
		else if (input_code == 0x51)	Alpha[alphabet::Q] = state;
		else if (input_code == 0x52)	Alpha[alphabet::R] = state;
		else if (input_code == 0x53)	Alpha[alphabet::S] = state;
		else if (input_code == 0x54)	Alpha[alphabet::T] = state;
		else if (input_code == 0x55)	Alpha[alphabet::U] = state;
		else if (input_code == 0x56)	Alpha[alphabet::V] = state;
		else if (input_code == 0x57)	Alpha[alphabet::W] = state;
		else if (input_code == 0x58)	Alpha[alphabet::X] = state;
		else if (input_code == 0x59)	Alpha[alphabet::Y] = state;
		else if (input_code == 0x5A)	Alpha[alphabet::Z] = state;
	}
	else if (input_code == VK_SHIFT)	Shift = state;
	else if (input_code == VK_CONTROL)	Ctrl = state;
	else if (input_code == VK_MENU)		Alt = state;
	else if (input_code == VK_TAB)		Tab = state;
	else if (input_code == VK_BACK)		Backspace = state;
	else if (input_code == VK_DELETE)	Delete = state;
	else if (input_code == VK_UP)		Arrow[arrow::UP] = state;
	else if (input_code == VK_DOWN)		Arrow[arrow::DOWN] = state;
	else if (input_code == VK_RIGHT)	Arrow[arrow::RIGHT] = state;
	else if (input_code == VK_LEFT)		Arrow[arrow::LEFT] = state;
}

void Input::SetInputs(bool state, unsigned long long data1, long long data2)
{
	if ((data1 & MK_LBUTTON) == MK_LBUTTON)	Mouse[mouse_control::LMB] = state;
	if ((data1 & MK_MBUTTON) == MK_MBUTTON)	Mouse[mouse_control::MMB] = state;
	if ((data1 & MK_RBUTTON) == MK_RBUTTON)	Mouse[mouse_control::RMB] = state;

	SetMousePos(data2);

	//if (!Mouse[LMB] && !Mouse[MMB] && !Mouse[RMB])
	//{
	//	ReleaseCapture();
	//}
	//else
	//{
	//	SetCapture(hMainWindow);
	//}
}

void Input::SetScroll(unsigned long long data1, long long data2)
{
	short delta = HIWORD(data1);

	if (delta > 0) Mouse[mouse_control::UP_SCROLL] = true;
	if (delta < 0) Mouse[mouse_control::DOWN_SCROLL] = true;
	if (delta == 0)
	{
		Mouse[mouse_control::UP_SCROLL] = false;
		Mouse[mouse_control::DOWN_SCROLL] = false;
	}
}

void Input::SetMousePos(long long data)
{
	short last_x = Mouse_x;
	short last_y = Mouse_y;

	Mouse_x = LOWORD(data) + 1;
	Mouse_y = HIWORD(data) + 1;

	if (last_x != Mouse_x || last_y != Mouse_y) Mouse[mouse_control::MOVE] = true;
	else										Mouse[mouse_control::MOVE] = false;
}


void Input::ReleaseMouseControls()
{
	Mouse[mouse_control::LMB] = false;
	Mouse[mouse_control::MMB] = false;
	Mouse[mouse_control::RMB] = false;
}

void Input::UpdateInputs()
{
	Mouse[UP_SCROLL] = false;
	Mouse[DOWN_SCROLL] = false;
}

