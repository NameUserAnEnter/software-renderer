#ifndef UTILS_H
#define UTILS_H

#include <windows.h>
#include <string>
#include <vector>

inline std::wstring toWide(std::string str) {
	std::wstring wide = L"";
	for (auto c : str) {
		wide += (wchar_t) c;
	}
	return wide;
}

inline std::string toNarrow(std::wstring str) {
	std::string narrow = "";
	for (auto c : str) {
		narrow += (unsigned char) c;
	}
	return narrow;
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

inline void Popup(std::wstring message) {
	MessageBoxW(NULL, message.c_str(), L"", MB_OK);
}

inline void Popup(std::string message) {
	MessageBoxA(NULL, message.c_str(), "", MB_OK);
}

inline void Popup(float num) {
	Popup(NumStr(num));
}

inline void Popup() {
	Popup("");
}

inline void SetWindowTitle(HWND hWindow, std::string title) {
	SetWindowTextA(hWindow, title.c_str());
}

inline std::string GetFileData(const wchar_t* filename) {
	std::string data = "";

	FILE* file;
	_wfopen_s(&file, filename, L"rb");

	if (file == nullptr) {
		Popup(L"Cannot open file \"" + std::wstring(filename) + L"\".");
		return data;
	}

	int currentChar;
	do {
		currentChar = getc(file);

		if (currentChar != EOF) data += (unsigned char) currentChar;
	} while (currentChar != EOF);
	fclose(file);

	return data;
}

template<typename T>
inline std::vector<std::basic_string<T>> SplitByChar(std::basic_string<T> str, T delimiter, bool leaveDelimiters = false) {
	std::vector<std::basic_string<T>> parts;
	parts.push_back(std::basic_string<T>());

	for (int i = 0; i < str.size(); i++) {
		parts.back() += str[i];

		if (str[i] == delimiter) {
			if (!leaveDelimiters) parts.back().pop_back();

			parts.push_back(std::basic_string<T>());
		}
	}

	if (!parts.empty()) {
		if (parts.back().size() == 0) parts.pop_back();
	}
	return parts;
}

#endif

