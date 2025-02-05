#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <windows.h>
#include "ColorBlock.h"
#include "ColorBlockTransparent.h"
#include "utils.h"
#include "ColorDefs.h" // temporary

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

	unsigned int raster_unit_thickness;
public:
	void Init(HWND hWindow);

	void InitializeBuffers();
	void ReleaseBuffers();

	//		Rasterizer operations drawing on the backbuffer
	// Direct bitmap rasterizing
	void DrawBitmap(int x, int y, int width, int height, ColorBlockTransparent* bytes);

	// Core rasterizing
	void DrawPixel(int x, int y, ColorBlockTransparent color);
	
	void DrawPoint(int x, int y, ColorBlockTransparent color);
	void DrawLine(int x1, int y1, int x2, int y2, ColorBlockTransparent color);

	// Geometry outlines
	void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorBlockTransparent color);
	void DrawQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, ColorBlockTransparent color);

	// Filled geometry
	void DrawTriangleF(int x1, int y1, int x2, int y2, int x3, int y3, ColorBlockTransparent color);
	void DrawQuadF(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, ColorBlockTransparent color);

	void ClearBackBuffer();
	void UpdateFrontBuffer();

	void ResizeBuffers(int width, int height);

	bool SetRasterUnitThickness(unsigned int);
	unsigned int GetRasterUnitThickness();
private:
	void update_buffer(HDC destination, HDC source);
	bool within_window(int x, int y);
};

#endif

