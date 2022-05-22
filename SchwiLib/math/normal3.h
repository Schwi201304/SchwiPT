#pragma once

#include "vector3.h"
namespace schwi {
	template<typename T>
	class Normal3 :public vec3<T> {
	public:
		Normal3() :vec3<T>() {}
		Normal3(T _x, T _y, T _z) :vec3<T>(_x, _y, _z) {}
		template <typename U>
		explicit Normal3(const vec3<U>& v) :vec3<T>(v) {}

		template<typename U>
		explicit operator Vector3<U>() const {
			return Vector3<U>(this->x, this->y, this->z);
		}

		Normal3<T> operator-() const {
			return Normal3<T>(-this->x, -this->y, -this->z);
		}

		double LengthSquared()const {
			return this->x * this->x + this->y * this->y + this->z * this->z;
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

	template <typename T>
	inline double Dot(const Vector3<T>& n, const Normal3<T>& v) {
		return n.x * v.x + n.y * v.y + n.z * v.z;
	}

	template<typename T>
	inline double AbsDot(const Vector3<T>& v, const Normal3<T>& n) {
		return std::abs(Dot(v, n));
	}

	template<typename T>
	inline double AbsDot(const Normal3<T>& n, const Vector3<T>& v) {
		return std::abs(Dot(v, n));
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
		return Normal3<T1>(t * v.x, t * v.y, t * v.z);
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

	template<typename T>
	inline Vector3<T> operator+(const Normal3<T>& u, const Vector3<T>& v) {
		return Vector3<T>(u.x + v.x, u.y + v.y, u.z + v.z);
	}

	template<typename T>
	inline Vector3<T> operator+(const Vector3<T>& u, const Normal3<T>& v) {
		return Vector3<T>(u.x + v.x, u.y + v.y, u.z + v.z);
	}
}
