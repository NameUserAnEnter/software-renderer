#ifndef UTILS_H
#define UTILS_H

#include <windows.h>
#include <string>

inline void PopupMessage(std::string message) {
	MessageBoxA(NULL, message.c_str(), "", MB_OK);
}

inline void PopupMessage(unsigned int num) {
	PopupMessage(std::to_string(num));
}

inline void PopupMessage(float num) {
	PopupMessage(std::to_string(num));
}

inline void SetTitle(HWND hWindow, std::string title) {
	SetWindowTextA(hWindow, title.c_str());
}

#endif

