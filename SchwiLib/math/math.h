#pragma once

#include<assert.h>
#include<cmath>
#include<iostream>
#include<limits>

namespace schwi {

	// Math Constants
	constexpr double Pi = 3.14159265358979323846;
	constexpr double InvPi = 1/Pi;
	constexpr double Inv2Pi = InvPi/2;
	constexpr double Inv4Pi = Inv2Pi/4;
	constexpr double PiOver2 = Pi/2;
	constexpr double PiOver4 = Pi/4;
	constexpr double Sqrt2 = 1.41421356237309504880;
	constexpr double Log2 = 1.44269504088896322870;

	constexpr double Infinity = std::numeric_limits<double>::max();
	constexpr double MachineEpsilon = std::numeric_limits<double>::epsilon() * 0.5;

	template <typename T> class vec2;
	template <typename T> class vec3;
	template <typename T> class vec4;
	template <typename T> class Vector2;
	template <typename T> class Vector3;
	template <typename T> class Point2;
	template <typename T> class Point3;
	template <typename T> class Normal3;
	template<typename T, int R, int C>class Matrix;

	using BYTE = unsigned char;
	using Degree = double;
	using Radian = double;

	constexpr int Dynamic = -1;
	using std::max;
	using std::min;
	using std::sqrt;
	using std::cos;
	using std::sin;
	using Matrix2d = Matrix<double, 2, 2>;
	using Matrix3d = Matrix<double, 3, 3>;
	using Matrix4d = Matrix<double, 4, 4>;
	using MatrixXd = Matrix<double, Dynamic, Dynamic>;

	using Normal3d = Normal3<double>;

	using Point2d = Point2<double>;
	using Point2i = Point2<int>;

	using Point3d = Point3<double>;
	using Point3i = Point3<int>;

	using Vector2i = Vector2<int>;
	using Vector2d = Vector2<double>;

	using Vector3i = Vector3<int>;
	using Vector3d = Vector3<double>;

	// Global Function
	inline Radian ToRadians(Degree deg) {
		return (Pi / 180) * deg;
	}

	inline Degree ToDegree(Radian rad) {
		return (180 * InvPi) * rad;
	}

	inline double Lerp(double t, double a, double b) {
		return (1 - t) * a + t * b;
	}

	inline double Clamp(double v, double l, double h) {
		return v > h ? h
			: v < l ? l
			: v;
	}

	inline double Clamp(double v) {
		return Clamp(v, 0, 1);
	}
}