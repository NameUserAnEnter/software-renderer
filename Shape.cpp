#include "Shape.h"

Shape::Shape() {
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	bytes = nullptr;
}

void Shape::Resize(int width, int height) {
	this->width = width;
	this->height = height;
}

void Shape::FillWithColor(ColorBlockTransparent color) {
	ReleaseShape();

	size_t byte_count = (size_t) (width * height);
	bytes = (ColorBlockTransparent*) calloc(byte_count, sizeof(ColorBlockTransparent));
	if (bytes != nullptr) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				bytes[i * width + j] = color;
			}
		}
	}
}

void Shape::FillWithColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A) {
	ColorBlockTransparent color { R, G, B, A };
	FillWithColor(color);
}

void Shape::ReleaseShape() {
	if (bytes != nullptr) {
		free(bytes);
	}
}

unsigned int Shape::GetWidth() {
	return width;
}

unsigned int Shape::GetHeight() {
	return height;
}

ColorBlockTransparent* Shape::GetBytes() {
	return bytes;
}

