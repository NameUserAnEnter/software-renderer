#ifndef POLYGON_H
#define POLYGON_H

#include "DimensionalTypes.h"

struct Quad {
public:
	float2 p1, p2, p3, p4;

	bool CoversOther(const Quad& other) {
		if (PointWithin(other.p1) && PointWithin(other.p2) && PointWithin(other.p3) && PointWithin(other.p4)) return true;
		return false;
	}

	bool PointWithin(const float2& point) {
		// ray-casting point-in-polygon algorithm ...

		//// rectangle algorithm
		//auto xmax = fmax(p1.x, p2.x); xmax = fmax(xmax, p3.x); xmax = fmax(xmax, p4.x);
		//auto ymax = fmax(p1.y, p2.y); ymax = fmax(ymax, p3.y); ymax = fmax(ymax, p4.y);

		//auto xmin = fmin(p1.x, p2.x); xmin = fmin(xmin, p3.x); xmin = fmin(xmin, p4.x);
		//auto ymin = fmin(p1.y, p2.y); ymin = fmin(ymin, p3.y); ymin = fmin(ymin, p4.y);

		//if (point.x > xmax || point.x < xmin) return false;
		//if (point.y > ymax || point.y < ymin) return false;

		//// if this point is reached, means the point is within the quad's bounding box rectangle
		//// ...

		// sides:
		// p1 - p2
		// p2 - p3
		// p3 - p4
		// p4 - p1

		float2* sides[4][2];

		sides[0][0] = &p1;
		sides[0][1] = &p2;

		sides[1][0] = &p2;
		sides[1][1] = &p3;

		sides[2][0] = &p3;
		sides[2][1] = &p4;

		sides[3][0] = &p4;
		sides[3][1] = &p1;

		unsigned int crossed_sides = 0;
		for (int i = 0; i < 4; i++) {
			float2 a = *sides[i][0];
			float2 b = *sides[i][1];

			// casting the ray from point requested to the right

			// if both of the current side's points have their value of x < point.x, then the casted ray will not cross this side
			if (a.x < point.x && b.x < point.x) continue;

			// if point.y is not inbetween a.y and b.y and neither equal to a.y or by, then the casted ray will not cross this side
			if ((a.y < point.y && b.y < point.y) || (a.y > point.y && b.y > point.y)) continue;

			// reference calculation relies on a.x being > b.x
			if (a.x < b.x) {
				float2 temp = a;
				a = b;
				b = temp;
			}

			// assure no division by zero, geometrically this means point.y = a.y = b.y
			if ((a.y - b.y) == 0) {
				return true;
			}

			float2 c;
			c.y = point.y;
			c.x = (c.y - b.y) * ((a.x - b.x) / (a.y - b.y)) + b.x;

			if (point.x < c.x) continue;

			crossed_sides++;
		}

		// ray casting point-in-polygon algorithm rule; if the number of sides (e.g. 0, 2) crossed by the ray is even then the point is outside of the polygon
		if (crossed_sides % 2 == 0) return false;
		return true;
	}
};

struct Rectangle {
	float2 p1, p2, p3, p4;

	bool CoversOther(const Quad& other) {
		if (PointWithin(other.p1) && PointWithin(other.p2) && PointWithin(other.p3) && PointWithin(other.p4)) return true;
		return false;
	}

	bool PointWithin(const float2& point) {
		// rectangle algorithm
		auto xmax = fmax(p1.x, p2.x); xmax = fmax(xmax, p3.x); xmax = fmax(xmax, p4.x);
		auto ymax = fmax(p1.y, p2.y); ymax = fmax(ymax, p3.y); ymax = fmax(ymax, p4.y);

		auto xmin = fmin(p1.x, p2.x); xmin = fmin(xmin, p3.x); xmin = fmin(xmin, p4.x);
		auto ymin = fmin(p1.y, p2.y); ymin = fmin(ymin, p3.y); ymin = fmin(ymin, p4.y);

		if (point.x > xmax || point.x < xmin) return false;
		if (point.y > ymax || point.y < ymin) return false;

		return true;
	}

	static Rectangle BoundingBox(const Quad& q) {
		auto xmax = fmax(q.p1.x, q.p2.x); xmax = fmax(xmax, q.p3.x); xmax = fmax(xmax, q.p4.x);
		auto ymax = fmax(q.p1.y, q.p2.y); ymax = fmax(ymax, q.p3.y); ymax = fmax(ymax, q.p4.y);

		auto xmin = fmin(q.p1.x, q.p2.x); xmin = fmin(xmin, q.p3.x); xmin = fmin(xmin, q.p4.x);
		auto ymin = fmin(q.p1.y, q.p2.y); ymin = fmin(ymin, q.p3.y); ymin = fmin(ymin, q.p4.y);

		Rectangle r = {
			{ xmin, ymin },
			{ xmax, ymin },
			{ xmax, ymax },
			{ xmin, ymax }
		};

		return r;
	}
};

#endif

