#pragma once

#include "vector3.h"
namespace schwi {
	template<typename T>
	class Normal3 :public vec3<T> {
	public:
		Normal3() :vec2() {}
		Normal3(T _x, T _y, T _z) :vec3<T>(_x, _y, _z) {}
		explicit Normal3(const Vector3<T>& v) :vec3<T>(v.x, v.y, v.z) {}

		Normal3<T> operator-() const { return Normal3<T>(-this->x, -this->y, -this->z); }

		double LengthSquared()const{
			return x * x + y * y + z * z;
		}

		double Length()const {
			return std::sqrt(LengthSquared());
		}

		Normal3<T> Normalize()const {
			return (*this) / Length();
		}
	};


	template <typename T>
	inline double Dot(const Normal3<T>& n, const Vector3<T>& v) {
		return n.x * v.x + n.y * v.y + n.z * v.z;
	}

	template<typename T>
	inline Normal3<double> Cross(const Normal3<T>& u, const Normal3<T>& v) {
		return Normal3<double>(u.y * v.z - u.z * v.y,
			u.z * v.x - u.x * v.z,
			u.x * v.y - u.y * v.x);
	}

	template <typename T>
	inline Normal3<T> Faceforward(const Normal3<T>& n, const Vector3<T>& v) {
		return (Dot(n, v) < 0.) ? -n : n;
	}

	template<typename T1, typename T2>
	inline Normal3<T1> operator*(T2 t, const Normal3<T1>& v) {
		return Vector3<T1>(t * v.x, t * v.y, t * v.z);
	}

	template<typename T1, typename T2>
	inline Normal3<T1> operator*(const Normal3<T1>& v, T2 t) {
		return t * v;
	}

	template<typename T1, typename T2>
	inline Normal3<T1> operator/(const Normal3<T1>& v, T2 t) {
		assert(t != 0);
		return (1 / t) * v;
	}
}
