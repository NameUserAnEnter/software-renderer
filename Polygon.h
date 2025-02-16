#ifndef POLYGON_H
#define POLYGON_H

#include "DimensionalTypes.h"

struct Polygon {
	std::vector<float2> vertices;

	float2& operator[](unsigned int index) {
		if (index >= vertices.size()) vertices.resize(index + 1);

		return vertices[index];
	}

	bool CoversOther(Polygon& other) {
		for (int i = 0; i < other.vertices.size(); i++) {
		//for (int i = 0; i < 1; i++) {
			if (!PointWithin(other.vertices[i])) return false;
		}

		return true;
	}

	bool PointWithin(const float2& point) {
		// ray-casting point-in-polygon algorithm

		std::vector<std::vector<float2>> sides;

		for (int i = 1; i <= vertices.size(); i++) {
			int i1 = i - 1;
			int i2 = i % vertices.size();

			sides.push_back({ vertices[i1], vertices[i2] });
		}

		unsigned int crossed_sides = 0;
		for (int i = 0; i < sides.size(); i++) {
			float2 a = sides[i][0];
			float2 b = sides[i][1];

			// casting the ray from point requested to the right

			// if both of the current side's points have their value of x < point.x, then the casted ray will not cross this side
			if (a.x < point.x && b.x < point.x) continue;

			// if point.y is not inbetween a.y and b.y and neither equal to a.y or by, then the casted ray will not cross this side
			if ((a.y < point.y && b.y < point.y) || (a.y > point.y && b.y > point.y)) continue;
			
			// at this moment point.y is in the range [a.y; b.y]

			// assure no division by zero, geometrically this means point.y = a.y = b.y, so point is on the current side, so in the polygon
			if ((a.y - b.y) == 0) {
				return true;
			}

			// make sure crossing corners does not count as crossing two sides
			if (b.y == point.y) continue;

			// further calculations assume a.x >= b.x, so if this is not true swap a and b
			if (a.x < b.x) {
				float2 temp = a;
				a = b;
				b = temp;
			}

			// c is a point on the current side with y equal to point.y
			float2 c;
			c.y = point.y;
			c.x = (c.y - b.y) * ((a.x - b.x) / (a.y - b.y)) + b.x;

			// point.x == c.x means point is on the current side, so in the polygon
			if (point.x == c.x) return true;

			// point.x > c.x means ray does not cross the current side
			if (point.x > c.x) continue;

			crossed_sides++;
		}

		// method testing
		//Popup(crossed_sides);

		// ray casting point-in-polygon algorithm rule; if the number of sides crossed by the ray is even (e.g. 0, 2) then the point is outside of the polygon
		if (crossed_sides % 2 == 0) return false;
		return true;
	}

	static Polygon BoundingBox(Polygon& other) {
		float ymin, xmin;
		float xmax, ymax;

		for (int i = 0; i < other.vertices.size(); i++) {
			if (i == 0) {
				xmin = other.vertices[i].x;
				xmax = other.vertices[i].x;

				ymin = other.vertices[i].y;
				ymax = other.vertices[i].y;

				continue;
			}

			if (other.vertices[i].x < xmin) xmin = other.vertices[i].x;
			if (other.vertices[i].x > xmax) xmax = other.vertices[i].x;

			if (other.vertices[i].y < ymin) ymin = other.vertices[i].y;
			if (other.vertices[i].y > ymax) ymax = other.vertices[i].y;
		}

		return {{
			{ xmin, ymin },
			{ xmax, ymin },
			{ xmax, ymax },
			{ xmin, ymax },
		}};
	}
};

#endif

