#ifndef DIMENSIONALTYPES_H
#define DIMENSIONALTYPES_H

template<typename T> struct xy {
	T x, y;

	bool operator==(xy operand) {
		if (x == operand.x && y == operand.y) return true;
		else return false;
	}

	bool operator!=(xy operand) {
		if (*this == operand) return false;
		else return true;
	}
};

template<typename T> struct xyz {
	T x, y, z;

	bool operator==(xyz operand) {
		if (x == operand.x && y == operand.y && z == operand.z) return true;
		else return false;
	}

	bool operator!=(xyz operand) {
		if (*this == operand) return false;
		else return true;
	}
};

typedef xy<int> int2;
typedef xy<float> float2;
typedef xyz<float> float3;

#endif

