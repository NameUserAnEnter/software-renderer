#ifndef UTILS_H
#define UTILS_H

#include <windows.h>
#include <string>
#include <vector>
#include "DimensionalTypes.h"

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

inline std::string NumStr(float num, unsigned int minimal_length) {
	std::string str = NumStr(num);

	while (str.size() < minimal_length) {
		str = ' ' + str;
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

template<typename T>
inline std::vector<T> Quicksort(std::vector<T> data) {
	if (data.size() <= 1) return data;

	std::vector<T> h1; // first half
	std::vector<T> h2; // second half

	//								   = expected
	//						  -> formula
	//					 -> size
	//				-> expected
	// indices
	// 0 1 2 3 4 5	-> 3 -> 6 -> 6 / 2 = 3
	// 0 1 2 3 4	-> 2 -> 5 -> 5 / 2 = 2
	// 0 1 2 3		-> 2 -> 4 -> 4 / 2 = 2
	// 0 1 2		-> 1 -> 3 -> 3 / 2 = 1
	// 0 1			-> 1 -> 2 -> 2 / 2 = 1

	unsigned int pivot_point = data.size() / 2;

	for (int i = 0; i < data.size(); i++) {
		if (i == pivot_point) {
			if (h1.size() >= h2.size()) h2.push_back(data[i]);
			else						h1.push_back(data[i]);
		}
		else if (data[i] >  data[pivot_point]) h2.push_back(data[i]);
		else if (data[i] <= data[pivot_point]) h1.push_back(data[i]);
	}

	h1 = Quicksort(h1);
	h2 = Quicksort(h2);

	std::vector<T> combined;
	for (auto e : h1) combined.push_back(e);
	for (auto e : h2) combined.push_back(e);

	return combined;
}

template <typename T1, typename T2>
inline std::vector<std::pair<T1, T2>> QuicksortMap(std::vector<std::pair<T1, T2>> data) {
	if (data.size() <= 1) return data;

	std::vector<std::pair<T1, T2>> h1, h2;
	unsigned int pivot_point = data.size() / 2;

	for (int i = 0; i < data.size(); i++) {
		if (i == pivot_point) {
			if (h1.size() >= h2.size()) h2.push_back(data[i]);
			else						h1.push_back(data[i]);
		}
		else if (data[i].second >  data[pivot_point].second) h2.push_back(data[i]);
		else if (data[i].second <= data[pivot_point].second) h1.push_back(data[i]);
	}

	h1 = QuicksortMap(h1);
	h2 = QuicksortMap(h2);
	
	std::vector<std::pair<T1, T2>> combined;
	for (auto e : h1) combined.push_back(e);
	for (auto e : h2) combined.push_back(e);

	return combined;
}

template<typename T>
inline bool Contains(std::vector<T> data, T element) {
	for (auto e : data) {
		if (e == element) return true;
	}
	return false;
}

template<typename T>
inline std::vector<T> Reverse(std::vector<T> data) {
	std::vector<T> reversed;
	for (int i = 0; i < data.size(); i++) {
		reversed.push_back(data[data.size() - 1 - i]);
	}
	return reversed;
}

#endif

