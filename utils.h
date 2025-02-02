#ifndef UTILS_H
#define UTILS_H

#include <windows.h>
#include <string>

inline void PopupMessage(std::string message) {
	MessageBoxA(NULL, message.c_str(), "", MB_OK);
}

#endif

