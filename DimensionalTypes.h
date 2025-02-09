#ifndef DIMENSIONALTYPES_H
#define DIMENSIONALTYPES_H

namespace DimensionalTypes {
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

		// to be tested
		void operator+=(float operand) {
			x += operand;
			y += operand;
			z += operand;
		}

		void operator-=(float operand) {
			x -= operand;
			y -= operand;
			z -= operand;
		}

		xyz operator-() {
			return { -x, -y, -z };
		}
	};
}

typedef DimensionalTypes::xy<int> int2;
typedef DimensionalTypes::xy<float> float2;
typedef DimensionalTypes::xyz<float> float3;

#endif

