#pragma once

// Global Include Files
#include<algorithm>
#include<assert.h>
#include<cmath>
#include<iostream>
#include<limits>
#include<memory>
#include<string>
#include<vector>
#include<initializer_list>
#include<typeinfo>

namespace schwi {
	//Global Constants
	static constexpr double Pi = 3.14159265358979323846;
	static constexpr double InvPi = 0.31830988618379067154;
	static constexpr double Inv2Pi = 0.15915494309189533577;
	static constexpr double Inv4Pi = 0.07957747154594766788;
	static constexpr double PiOver2 = 1.57079632679489661923;
	static constexpr double PiOver4 = 0.78539816339744830961;
	static constexpr double Sqrt2 = 1.41421356237309504880;
	static constexpr double Log2 = 1.44269504088896322870;

	static constexpr double Infinity = std::numeric_limits<double>::max();
	static constexpr double MachineEpsilon = std::numeric_limits<double>::epsilon() * 0.5;

	// Using
	using Degree = double;
	using Radian = double;


	// Global Forward Declarations
	template <typename T> class vec2;
	template <typename T> class vec3;
	template <typename T> class vec4;
	template <typename T> class Vector2;
	template <typename T> class Vector3;
	template <typename T> class Point2;
	template <typename T> class Point3;
	template <typename T> class Normal3;
	template <typename T> class Bounds3;
	class Camera;
	class Shape;
	class Ray;

	// Global Function
	inline Radian ToRadians(Degree deg) {
		return (Pi / 180) * deg;
	}

	inline Degree Degrees(Radian rad) {
		return (180 * InvPi) * rad;
	}

	inline double Lerp(double t, double a, double b) {
		return (1 - t) * a + t * b;
	}
}