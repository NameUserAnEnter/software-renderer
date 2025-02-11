#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <windows.h>
#include "ColorBlock.h"
#include "ColorBlockTransparent.h"
#include "utils.h"
#include "ColorDefs.h" // temporary
#include "DimensionalTypes.h"

class Graphics {
public:
	static enum TEXT_ORIENTATION { LEFT, RIGHT, CENTER };

public:
	Graphics();
	~Graphics();

	void Init(HWND hWindow);

	void InitializeBuffers();
	void ReleaseBuffers();

	void ClearBackBuffer();
	void UpdateFrontBuffer();

private:
	void update_buffer(HDC destination, HDC source);
	bool within_window(int x, int y);

public:
	void ResizeBuffers(int width, int height);

	bool SetRasterUnitThickness(unsigned int);
	unsigned int GetRasterUnitThickness();

	void Print(std::string, int, int, TEXT_ORIENTATION = TEXT_ORIENTATION::LEFT);
	void Print(std::wstring, int, int, TEXT_ORIENTATION = TEXT_ORIENTATION::LEFT);

	//		Rasterizer operations drawing on the backbuffer
	// Direct bitmap rasterizing
	void DrawBitmap(int, int, int, int, ColorBlock*);
	void DrawBitmap(int2, int2, ColorBlock*);

	//		Core rasterizing
	void DrawPixel(int, int, ColorBlock);
	void DrawPixel(int2, ColorBlock);
	
	void DrawPoint(int, int, ColorBlock);
	void DrawPoint(int2, ColorBlock);

	void DrawLine(int, int, int, int, ColorBlock);
	void DrawLine(int2, int2, ColorBlock);

	// Geometry outlines
	void DrawTriangle(int, int, int, int, int, int, ColorBlock);
	void DrawTriangle(int2, int2, int2, ColorBlock);

	void DrawQuad(int, int, int, int, int, int, int, int, ColorBlock);
	void DrawQuad(int2, int2, int2, int2, ColorBlock);

	// Filled geometry
	void FillTriangle(int, int, int, int, int, int, ColorBlock);
	void FillTriangle(int2, int2, int2, ColorBlock);

	void FillQuad(int, int, int, int, int, int, int, int, ColorBlock);
	void FillQuad(int2, int2, int2, int2, ColorBlock);
	//		---

private:
	HWND hWindow;

	unsigned int uBufferWidth;
	unsigned int uBufferHeight;

	HDC frontbuffer_dc;
	HDC backbuffer_dc;

	HFONT font;

	HBITMAP cleanbuffer_bitmap;
	HBITMAP backbuffer_bitmap;

	BITMAPINFOHEADER backbuffer_bitmapinfo;

	size_t backbuffer_memory_block_size;
	unsigned char* backbuffer_bytes;

	unsigned int bytes_per_pixel;

	unsigned int raster_unit_thickness;
};

#endif

