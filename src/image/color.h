#pragma once

#include<core/schwi.h>
#include<image/image.h>
#include<math/vector3.h>

namespace schwi {
	class Color{
	public:
		double r, g, b;

	public:
		Color() = default;
		Color(double r, double g, double b)
			:r(r),g(g),b(b){}

		Color operator*(const double d) {
			return Color(r * d, g * d, b * d);
		}

		Color operator+(const double d) {
			return Color(r + d, g + d, b + d);
		}
	};

	inline SchwiColor ToByte(Color c) {
		return SchwiColor(c.r * 256, c.g * 256, c.b * 256);
	}

	inline Vector3d ToV3d(Color c) {
		return Vector3d(c.r, c.g, c.b);
	}

	inline Color ToColor(SchwiColor sc) {
		return Color(sc[0] / 256, sc[1] / 256, sc[2] / 256);
	}

	inline Color Lerp(double t, Color c1, Color c2) {
		return Color(Lerp(t, c1.r, c2.r), Lerp(t, c1.g, c2.g), Lerp(t, c1.b, c2.b));
	}
}