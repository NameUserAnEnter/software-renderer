#ifndef FLOAT_H
#define FLOAT_H

struct float2 {
	float x, y;

	bool operator==(float2 operand) {
		if (x == operand.x && y == operand.y) return true;
		else return false;
	}

	bool operator!=(float2 operand) {
		if (*this == operand) return false;
		else return true;
	}
};

struct float3 {
	float x, y, z;

	bool operator==(float3 operand) {
		if (x == operand.x && y == operand.y && z == operand.z) return true;
		else return false;
	}

	bool operator!=(float3 operand) {
		if (*this == operand) return false;
		else return true;
	}
};

#endif

