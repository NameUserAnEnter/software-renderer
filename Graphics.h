#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <windows.h>
#include "ColorBlock.h"
#include "ColorBlockTransparent.h"
#include "utils.h"

class Graphics {
private:
	HWND hWindow;

	unsigned int uBufferWidth;
	unsigned int uBufferHeight;

	HDC frontbuffer_dc;
	HDC backbuffer_dc;

	HBITMAP cleanbuffer_bitmap;
	HBITMAP backbuffer_bitmap;

	BITMAPINFOHEADER backbuffer_bitmapinfo;

	size_t backbuffer_memory_block_size;
	unsigned char* backbuffer_bytes;

	unsigned int bytes_per_pixel;
public:
	Graphics(HWND hWindow);

	void InitializeBuffers();
	void ReleaseBuffers();

	// Rasterizer operations drawing on the backbuffer
	void DrawRectangle(int x, int y, int width, int height, ColorBlockTransparent* bytes);
	void DrawPixel(int x, int y, ColorBlockTransparent color);
	void DrawLine(int x1, int y1, int x2, int y2, ColorBlockTransparent);

	void ClearBackBuffer();
	void UpdateFrontBuffer();

	void ResizeBuffers(int width, int height);
private:
	void update_buffer(HDC destination, HDC source);
	bool within_window(int x, int y);
};

#endif

