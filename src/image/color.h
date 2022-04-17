#pragma once

#include<core/schwi.h>
#include<image/image.h>
#include<math/vector3.h>

namespace schwi {
	class Color {
	public:
		double r, g, b;

	public:
		Color() = default;
		Color(double r, double g, double b)
			:r(r), g(g), b(b) {}

		Color operator*(double d) const {
			return Color(r * d, g * d, b * d);
		}

		Color operator+(double d) const {
			return Color(r + d, g + d, b + d);
		}

		Color operator+(const Color& c) {
			return Color(r + c.r, g + c.g, b + c.b);
		}
	};

	inline Color operator*(double d, const Color& c) {
		return c * d;
	}

	inline Color operator/(const Color& c, double d) {
		return 1 / d * c;
	}

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
		return (1 - t) * c1 + t * c2;
	}
}