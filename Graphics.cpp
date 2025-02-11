#include "graphics.h"

Graphics::Graphics() {
	backbuffer_bytes = nullptr;
}

Graphics::~Graphics() {
	if (backbuffer_bytes != nullptr) {
		free(backbuffer_bytes);
		backbuffer_bytes = nullptr;
	}
}

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

	// font init
	font = CreateFontW(18, 8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
					  (DEFAULT_PITCH | FF_DONTCARE), TEXT("Consolas"));

	if (SetTextColor(backbuffer_dc, RGB(255, 255, 255)) == CLR_INVALID) Popup("Failed to invoke SetTextColor()");
	SetBkMode(backbuffer_dc, TRANSPARENT);

	if (font != NULL) SelectObject(backbuffer_dc, font);
}

void Graphics::ReleaseBuffers() {
	free(backbuffer_bytes);
	backbuffer_bytes = nullptr;

	if (font != NULL) DeleteObject(font);

	DeleteObject(cleanbuffer_bitmap);
	DeleteObject(backbuffer_bitmap);
	DeleteDC(backbuffer_dc);
}

void Graphics::ClearBackBuffer() {
	GetDIBits(frontbuffer_dc, cleanbuffer_bitmap, 0, uBufferHeight, backbuffer_bytes, (BITMAPINFO*)&backbuffer_bitmapinfo, DIB_RGB_COLORS);
	SetDIBits(NULL, backbuffer_bitmap, 0, uBufferHeight, backbuffer_bytes, (BITMAPINFO*)&backbuffer_bitmapinfo, DIB_RGB_COLORS);
}

void Graphics::UpdateFrontBuffer() {
	update_buffer(frontbuffer_dc, backbuffer_dc);
}

void Graphics::update_buffer(HDC destination, HDC source) {
	BitBlt(destination, 0, 0, uBufferWidth, uBufferHeight, source, 0, 0, SRCCOPY);
}

bool Graphics::within_window(int x, int y) {
	if (x >= 0 && x < uBufferWidth && y >= 0 && y < uBufferHeight) return true;
	else return false;
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

void Graphics::Print(std::string str, int x, int y, TEXT_ORIENTATION orientation) {
	Print(toWide(str), x, y, orientation);
}

void Graphics::Print(std::wstring str, int x, int y, TEXT_ORIENTATION orientation) {
	RECT rect = { x, y, uBufferWidth - x, uBufferHeight - y };

	wchar_t* ptr = (wchar_t*) calloc(str.size() + 1, sizeof(wchar_t));
	if (ptr == nullptr) return;

	for (int i = 0; i < str.size(); i++) {
		ptr[i] = str[i];
	}
	ptr[str.size()] = L'\0';

	UINT orientation_flag = DT_LEFT;
	if (orientation == TEXT_ORIENTATION::ALIGN_LEFT) orientation_flag = DT_LEFT;
	if (orientation == TEXT_ORIENTATION::ALIGN_RIGHT) orientation_flag = DT_RIGHT;
	if (orientation == TEXT_ORIENTATION::ALIGN_CENTER) orientation_flag = DT_CENTER;

	DrawTextW(backbuffer_dc, ptr, str.size(), &rect, orientation_flag);
	free(ptr);
}

unsigned int Graphics::GetRasterUnitThickness() {
	return raster_unit_thickness;
}

void Graphics::DrawBitmap(int x, int y, int width, int height, ColorBlock* bytes) {
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
					backbuffer_bytes[i * (uBufferWidth * bytes_per_pixel) + (j * bytes_per_pixel) + 0] = bytes[(i - y) * width + (j - x)].B;
					backbuffer_bytes[i * (uBufferWidth * bytes_per_pixel) + (j * bytes_per_pixel) + 1] = bytes[(i - y) * width + (j - x)].G;
					backbuffer_bytes[i * (uBufferWidth * bytes_per_pixel) + (j * bytes_per_pixel) + 2] = bytes[(i - y) * width + (j - x)].R;
				}
				else break;
			}
		}
	}

	SetDIBits(NULL, backbuffer_bitmap, 0, uBufferHeight, backbuffer_bytes, (BITMAPINFO*)&backbuffer_bitmapinfo, DIB_RGB_COLORS);
}

void Graphics::DrawBitmap(int2 pos, int2 size, ColorBlock* bytes) {
	DrawBitmap(pos.x, pos.y, size.x, size.y, bytes);
}

void Graphics::DrawPixel(int x, int y, ColorBlock color) {
	SetPixel(backbuffer_dc, x, y, RGB(color.R, color.G, color.B));
}

void Graphics::DrawPixel(int2 pos, ColorBlock color) {
	DrawPixel(pos.x, pos.y, color);
}

void Graphics::DrawPoint(int x, int y, ColorBlock color) {
	HPEN backbuffer_pen = CreatePen(PS_SOLID, raster_unit_thickness, RGB(color.R, color.G, color.B));
	HGDIOBJ original_pen = SelectObject(backbuffer_dc, backbuffer_pen);

	MoveToEx(backbuffer_dc, x, y, NULL);
	LineTo(backbuffer_dc, x + 1, y);
	LineTo(backbuffer_dc, x + 1, y + 1);
	LineTo(backbuffer_dc, x,     y + 1);
	LineTo(backbuffer_dc, x,     y);

	SelectObject(backbuffer_dc, original_pen);
	DeleteObject(backbuffer_pen);
}

void Graphics::DrawPoint(int2 pos, ColorBlock color) {
	DrawPoint(pos.x, pos.y, color);
}

void Graphics::DrawLine(int x1, int y1, int x2, int y2, ColorBlock color) {
	HPEN backbuffer_pen = CreatePen(PS_SOLID, raster_unit_thickness, RGB(color.R, color.G, color.B));
	HGDIOBJ original_pen = SelectObject(backbuffer_dc, backbuffer_pen);
	
	MoveToEx(backbuffer_dc, x1, y1, NULL);
	LineTo(backbuffer_dc, x2, y2);

	SelectObject(backbuffer_dc, original_pen);
	DeleteObject(backbuffer_pen);
}

void Graphics::DrawLine(int2 p1, int2 p2, ColorBlock color) {
	DrawLine(p1.x, p1.y, p2.x, p2.y, color);
}

void Graphics::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorBlock color) {
	HPEN backbuffer_pen = CreatePen(PS_SOLID, raster_unit_thickness, RGB(color.R, color.G, color.B));
	HGDIOBJ original_pen = SelectObject(backbuffer_dc, backbuffer_pen);

	MoveToEx(backbuffer_dc, x1, y1, NULL);
	LineTo(backbuffer_dc, x2, y2);
	LineTo(backbuffer_dc, x3, y3);
	LineTo(backbuffer_dc, x1, y1);

	SelectObject(backbuffer_dc, original_pen);
	DeleteObject(backbuffer_pen);
}

void Graphics::DrawTriangle(int2 p1, int2 p2, int2 p3, ColorBlock color) {
	DrawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);
}

void Graphics::DrawQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, ColorBlock color) {
	HPEN backbuffer_pen = CreatePen(PS_SOLID, raster_unit_thickness, RGB(color.R, color.G, color.B));
	HGDIOBJ original_pen = SelectObject(backbuffer_dc, backbuffer_pen);

	MoveToEx(backbuffer_dc, x1, y1, NULL);
	LineTo(backbuffer_dc, x2, y2);
	LineTo(backbuffer_dc, x3, y3);
	LineTo(backbuffer_dc, x4, y4);
	LineTo(backbuffer_dc, x1, y1);

	SelectObject(backbuffer_dc, original_pen);
	DeleteObject(backbuffer_pen);
}

void Graphics::DrawQuad(int2 p1, int2 p2, int2 p3, int2 p4, ColorBlock color) {
	DrawQuad(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, color);
}

void Graphics::FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorBlock color) {
	HPEN backbuffer_pen = CreatePen(PS_SOLID, raster_unit_thickness, RGB(color.R, color.G, color.B));
	HGDIOBJ original_pen = SelectObject(backbuffer_dc, backbuffer_pen);

	// brush part is expermiental, check docs on brushes
	HBRUSH backbuffer_brush = CreateSolidBrush(RGB(color.R, color.G, color.B));
	HGDIOBJ original_brush = SelectObject(backbuffer_dc, backbuffer_brush);

	POINT p[] = { { x1, y1 }, { x2, y2 }, { x3, y3 } };
	Polygon(backbuffer_dc, p, 3);

	SelectObject(backbuffer_dc, original_pen);
	DeleteObject(backbuffer_pen);

	SelectObject(backbuffer_dc, original_brush);
	DeleteObject(backbuffer_brush);
}

void Graphics::FillTriangle(int2 p1, int2 p2, int2 p3, ColorBlock color) {
	FillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);
}

void Graphics::FillQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, ColorBlock color) {
	HPEN backbuffer_pen = CreatePen(PS_SOLID, raster_unit_thickness, RGB(color.R, color.G, color.B));
	HGDIOBJ original_pen = SelectObject(backbuffer_dc, backbuffer_pen);

	// brush part is expermiental, check docs on brushes
	HBRUSH backbuffer_brush = CreateSolidBrush(RGB(color.R, color.G, color.B));
	HGDIOBJ original_brush = SelectObject(backbuffer_dc, backbuffer_brush);

	POINT p[] = { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } };
	Polygon(backbuffer_dc, p, 4);

	SelectObject(backbuffer_dc, original_pen);
	DeleteObject(backbuffer_pen);

	SelectObject(backbuffer_dc, original_brush);
	DeleteObject(backbuffer_brush);
}

void Graphics::FillQuad(int2 p1, int2 p2, int2 p3, int2 p4, ColorBlock color) {
	FillQuad(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, color);
}

