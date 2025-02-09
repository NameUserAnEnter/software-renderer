#ifndef UTILS_H
#define UTILS_H

#include <windows.h>
#include <string>

inline void Popup(std::string message) {
	MessageBoxA(NULL, message.c_str(), "", MB_OK);
}

inline void Popup(unsigned int num) {
	Popup(std::to_string(num));
}

inline void Popup() {
	Popup("");
}

inline void Popup(float num) {
	Popup(std::to_string(num));
}

inline void SetWindowTitle(HWND hWindow, std::string title) {
	SetWindowTextA(hWindow, title.c_str());
}

inline bool StrContains(std::string str, char c) {
	for (char current : str) {
		if (current == c) return true;
	}
	return false;
}

inline std::string StrStripR(std::string str, char c) {
	while (str.size() > 0) {
		if (str.back() == c) str.pop_back();
		else break;
	}
	return str;
}

inline std::string NumStr(float num) {
	std::string str = std::to_string(num);
	if (StrContains(str, '.')) {
		str = StrStripR(str, '0');

		if (str.back() == '.') str.pop_back();
	}

	return str;
}

#endif

