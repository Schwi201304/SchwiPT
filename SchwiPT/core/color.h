#pragma once

#include<core/schwi.h>
#include<math/vector3.h>

namespace schwi {
	class Color {
	public:
		double r, g, b;

	public:
		Color() { r = g = b = .0; };
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

		Color operator*(const Color& c) {
			return Color(r * c.r, g * c.g, b * c.b);
		}

		void Clear() {
			r = g = b = 0;
		}

		bool IsBlack() const {
			return r <= 0 && g <= 0 && b <= 0;
		}

		double Luminance() const {
			return 0.212671 * r + 0.715160 * g + 0.072169 * b;
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const Color& c) {
		os << "Color: [" << c.r << ',' << c.g << ',' << c.b << ']';
		return os;
	}

	inline Color operator*(double d, const Color& c) {
		return c * d;
	}

	inline Color operator/(const Color& c, double d) {
		return 1 / d * c;
	}

	inline SchwiColor ToByte(Color c) {
		return SchwiColor(GammaEncoding(c.r), GammaEncoding(c.g), GammaEncoding(c.b));
	}

	inline Vector3d ToV3d(Color c) {
		return Vector3d(c.r, c.g, c.b);
	}

	inline Color ToColor(SchwiColor sc) {
		return Color(sc[0] / 255., sc[1] / 255., sc[2] / 255.);
	}

	inline Color Lerp(double t, Color c1, Color c2) {
		return (1 - t) * c1 + t * c2;
	}

	inline Color Clamp(Color c) {
		return Color(Clamp(c.r), Clamp(c.g), Clamp(c.b));
	}
}