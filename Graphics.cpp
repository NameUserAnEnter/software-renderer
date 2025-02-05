#include "graphics.h"

void Graphics::Init(HWND hWindow) {
	this->hWindow = hWindow;

	bytes_per_pixel = 4;
	backbuffer_bytes = nullptr;

	raster_unit_thickness = 1;
}

void Graphics::InitializeBuffers() {
	frontbuffer_dc = GetDC(hWindow);

	backbuffer_dc = CreateCompatibleDC(frontbuffer_dc);
	backbuffer_bitmap = CreateCompatibleBitmap(frontbuffer_dc, uBufferWidth, uBufferHeight);

	SelectObject(backbuffer_dc, backbuffer_bitmap);

	cleanbuffer_bitmap = CreateCompatibleBitmap(frontbuffer_dc, uBufferWidth, uBufferHeight);

	update_buffer(backbuffer_dc, frontbuffer_dc);

	backbuffer_bytes = nullptr;

	backbuffer_bitmapinfo.biSize			= sizeof(BITMAPINFOHEADER);
	backbuffer_bitmapinfo.biWidth			= uBufferWidth;
	backbuffer_bitmapinfo.biHeight			= uBufferHeight * (-1);
	backbuffer_bitmapinfo.biPlanes			= 1;
	backbuffer_bitmapinfo.biBitCount		= 8 * bytes_per_pixel;
	backbuffer_bitmapinfo.biCompression		= BI_RGB;

	backbuffer_memory_block_size = (size_t)uBufferWidth * bytes_per_pixel * (size_t)uBufferHeight;
	backbuffer_bytes = (unsigned char*) calloc(backbuffer_memory_block_size, sizeof(unsigned char));

	GetDIBits(frontbuffer_dc, backbuffer_bitmap, 0, uBufferHeight, backbuffer_bytes, (BITMAPINFO*)&backbuffer_bitmapinfo, DIB_RGB_COLORS);
	SetDIBits(NULL, cleanbuffer_bitmap, 0, uBufferHeight, backbuffer_bytes, (BITMAPINFO*)&backbuffer_bitmapinfo, DIB_RGB_COLORS);
}

void Graphics::ReleaseBuffers() {
	if (backbuffer_bytes != nullptr) {
		free(backbuffer_bytes);
	}

	DeleteObject(cleanbuffer_bitmap);
	DeleteObject(backbuffer_bitmap);
	DeleteDC(backbuffer_dc);
}

void Graphics::DrawBitmap(int x, int y, int width, int height, ColorBlockTransparent* bytes) {
	if (bytes == nullptr) {
		return;
	}

	for (int i = y; i < y + height; i++)
	{
		for (int j = x; j < x + width; j++)
		{
			if (within_window(j, i))
			{
				if ((j - x) >= 0 && (j - x) < width		&&		(i - y) >= 0 && (i - y) < height)
				{
					if (bytes[(i - y) * width + (j - x)].A != 0)
					{
						backbuffer_bytes[i * (uBufferWidth * bytes_per_pixel) + (j * bytes_per_pixel) + 0] = bytes[(i - y) * width + (j - x)].B;
						backbuffer_bytes[i * (uBufferWidth * bytes_per_pixel) + (j * bytes_per_pixel) + 1] = bytes[(i - y) * width + (j - x)].G;
						backbuffer_bytes[i * (uBufferWidth * bytes_per_pixel) + (j * bytes_per_pixel) + 2] = bytes[(i - y) * width + (j - x)].R;
					}
				}
				else break;
			}
		}
	}

	SetDIBits(NULL, backbuffer_bitmap, 0, uBufferHeight, backbuffer_bytes, (BITMAPINFO*)&backbuffer_bitmapinfo, DIB_RGB_COLORS);
}

void Graphics::DrawPixel(int x, int y, ColorBlockTransparent color) {
	SetPixel(backbuffer_dc, x, y, RGB(color.R, color.G, color.B));
}

void Graphics::DrawPoint(int x, int y, ColorBlockTransparent color) {
	HPEN backbuffer_pen = CreatePen(PS_SOLID, raster_unit_thickness, RGB(color.R, color.G, color.B));
	HGDIOBJ original_obj = SelectObject(backbuffer_dc, backbuffer_pen);

	MoveToEx(backbuffer_dc, x, y, NULL);
	LineTo(backbuffer_dc, 1, 1);

	SelectObject(backbuffer_dc, original_obj);
	DeleteObject(backbuffer_pen);
}

void Graphics::DrawLine(int x1, int y1, int x2, int y2, ColorBlockTransparent color) {
	HPEN backbuffer_pen = CreatePen(PS_SOLID, raster_unit_thickness, RGB(color.R, color.G, color.B));
	HGDIOBJ original_obj = SelectObject(backbuffer_dc, backbuffer_pen);
	
	MoveToEx(backbuffer_dc, x1, y1, NULL);
	LineTo(backbuffer_dc, x2, y2);

	SelectObject(backbuffer_dc, original_obj);
	DeleteObject(backbuffer_pen);
}

void Graphics::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorBlockTransparent color) {
	HPEN backbuffer_pen = CreatePen(PS_SOLID, raster_unit_thickness, RGB(color.R, color.G, color.B));
	HGDIOBJ original_obj = SelectObject(backbuffer_dc, backbuffer_pen);

	MoveToEx(backbuffer_dc, x1, y1, NULL);
	LineTo(backbuffer_dc, x2, y2);
	LineTo(backbuffer_dc, x3, y3);
	LineTo(backbuffer_dc, x1, y1);

	SelectObject(backbuffer_dc, original_obj);
	DeleteObject(backbuffer_pen);
}

void Graphics::DrawQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, ColorBlockTransparent color) {
	HPEN backbuffer_pen = CreatePen(PS_SOLID, raster_unit_thickness, RGB(color.R, color.G, color.B));
	HGDIOBJ original_obj = SelectObject(backbuffer_dc, backbuffer_pen);

	MoveToEx(backbuffer_dc, x1, y1, NULL);
	LineTo(backbuffer_dc, x2, y2);
	LineTo(backbuffer_dc, x3, y3);
	LineTo(backbuffer_dc, x4, y4);
	LineTo(backbuffer_dc, x1, y1);

	SelectObject(backbuffer_dc, original_obj);
	DeleteObject(backbuffer_pen);
}

void Graphics::DrawTriangleF(int x1, int y1, int x2, int y2, int x3, int y3, ColorBlockTransparent color) {
}

void Graphics::DrawQuadF(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, ColorBlockTransparent color) {
}

void Graphics::ClearBackBuffer() {
	GetDIBits(frontbuffer_dc, cleanbuffer_bitmap, 0, uBufferHeight, backbuffer_bytes, (BITMAPINFO*)&backbuffer_bitmapinfo, DIB_RGB_COLORS);
	SetDIBits(NULL, backbuffer_bitmap, 0, uBufferHeight, backbuffer_bytes, (BITMAPINFO*)&backbuffer_bitmapinfo, DIB_RGB_COLORS);
}

void Graphics::UpdateFrontBuffer() {
	update_buffer(frontbuffer_dc, backbuffer_dc);
}

void Graphics::ResizeBuffers(int width, int height) {
	uBufferWidth = width;
	uBufferHeight = height;

	ReleaseBuffers();
	InitializeBuffers();
}

bool Graphics::SetRasterUnitThickness(unsigned int t) {
	if (t > 10) return false;

	raster_unit_thickness = t;
	return true;
}

unsigned int Graphics::GetRasterUnitThickness() {
	return raster_unit_thickness;
}

void Graphics::update_buffer(HDC destination, HDC source) {
	BitBlt(destination, 0, 0, uBufferWidth, uBufferHeight, source, 0, 0, SRCCOPY);
}

bool Graphics::within_window(int x, int y) {
	if (x >= 0 && x < uBufferWidth && y >= 0 && y < uBufferHeight) return true;
	else return false;
}

