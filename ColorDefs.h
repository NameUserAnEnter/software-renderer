#ifndef COLORDEFS_H
#define COLORDEFS_H

#include "ColorBlock.h"
#include "ColorBlockTransparent.h"

namespace Color {
	static const ColorBlock white	= { 255, 255, 255 };

	static const ColorBlock yellow	= { 255, 255, 0 };
	static const ColorBlock magenta	= { 255, 0, 255 };
	static const ColorBlock cyan	= { 0, 255, 255 };

	static const ColorBlock red		= { 255, 0, 0 };
	static const ColorBlock green	= { 0, 255, 0 };
	static const ColorBlock blue	= { 0, 0, 255 };

	static const ColorBlock purple		= { 170, 0, 255 };
	static const ColorBlock dark_purple	= {  30, 0,  90 };
};

#endif

