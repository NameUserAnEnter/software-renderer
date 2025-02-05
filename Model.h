#ifndef MODEL_H
#define MODEL_H

#include "Float.h"
#include <vector>

class Model {
public:
	Model();

	void AddPoint(float3);
	void AddPoint(float x, float y, float z);

	std::vector<float3> Points();
	
	void Release();
private:
	// the point SET - order does not matter and, as assured by AddPoint(), there is no duplicate values
	// that way the model object is primitive-topology-agnostic
	std::vector<float3> points;
public:
	float3 pos;
	float scale;
	float3 angle;
};

#endif

