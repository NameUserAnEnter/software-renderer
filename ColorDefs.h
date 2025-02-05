#ifndef COLORDEFS_H
#define COLORDEFS_H

#include "ColorBlock.h"
#include "ColorBlockTransparent.h"

namespace Color {
	static const ColorBlockTransparent white	= { 255, 255, 255, 255 };

	static const ColorBlockTransparent yellow	= { 255, 255, 0, 255 };
	static const ColorBlockTransparent magenta	= { 255, 0, 255, 255 };
	static const ColorBlockTransparent cyan		= { 0, 255, 255, 255 };

	static const ColorBlockTransparent red		= { 255, 0, 0, 255 };
	static const ColorBlockTransparent green	= { 0, 255, 0, 255 };
	static const ColorBlockTransparent blue		= { 0, 0, 255, 255 };

	static const ColorBlockTransparent purple		= { 170, 0, 255, 255 };
	static const ColorBlockTransparent dark_purple	= {  30, 0,  90, 255 };
};

#endif

