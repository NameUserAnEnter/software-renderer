#ifndef SHAPE_H
#define SHAPE_H

#include "Graphics.h"
#include "ColorBlockTransparent.h"

class Shape {
public:
	Shape();

	void Resize(int width, int height);

	void FillWithColor(ColorBlockTransparent);
	void FillWithColor(unsigned char, unsigned char, unsigned char, unsigned char);
	void ReleaseShape();

	unsigned int GetWidth();
	unsigned int GetHeight();

	ColorBlockTransparent* GetBytes();
public:
	int x, y;
private:
	ColorBlockTransparent* bytes;
	unsigned int width, height;
};

#endif

