#ifndef MESHELEMENTS_H
#define MESHELEMENTS_H

#include "DimensionalTypes.h"
#include "ColorBlock.h"
#include "utils.h"
#include <vector>
#include <string>

struct Vertex {
	float3 pos;
	ColorBlock color = { 255, 255, 255 };

	void operator=(float3 f) {
		pos = f;
	}
};

struct Face {
	std::vector<int> vertexIndices;
	std::vector<int> textureIndices;
	std::vector<int> normalIndices;

	std::string toString() {
		std::string str = "f";
		for (int i = 0; i < vertexIndices.size(); i++) {
			str += " " + NumStr(vertexIndices[i]);

			if (i < textureIndices.size()) {
				str += "/" + NumStr(textureIndices[i]);

				if (i < normalIndices.size()) {
					str += "/" + NumStr(normalIndices[i]);
				}
			}
			else {
				if (i < normalIndices.size()) {
					str += "//" + NumStr(normalIndices[i]);
				}
			}
		}

		return str;
	}
};

#endif

