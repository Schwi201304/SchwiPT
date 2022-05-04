#pragma once

#include "vec.h"

namespace schwi {
	template<typename T>
	class Vector3 :public vec3<T> {
	public:
		Vector3() :vec3<T>() {};
		Vector3(T _x, T _y, T _z) :vec3<T>(_x, _y, _z) {}
		template <typename U>
		explicit Vector3(const vec3<U>& v) :vec3<T>(v) {}

		Vector3<T> operator-() const {
			return Vector3<T>(-this->x, -this->y, -this->z);
		}

		Vector3<T>& operator+=(const Vector3<T>& v) {
			this->x += v.x;
			this->y += v.y;
			this->z += v.z;
			return *this;
		}

		Vector3<T>& operator-=(const Vector3<T>& v) {
			this->x -= v.x;
			this->y -= v.y;
			this->z -= v.z;
			return *this;
		}

		template<typename T1>
		Vector3<T>& operator*=(const T1 t) {
			this->x *= t;
			this->y *= t;
			this->z *= t;
			return *this;
		}

		template<typename T1>
		Vector3<T>& operator/=(const T1 t) {
			assert(t != 0);
			return *this *= 1 / t;
		}

		double Length() const {
			return std::sqrt(LengthSquared());
		}

		double LengthSquared() const {
			return this->x * this->x + this->y * this->y + this->z * this->z;
		}

		bool near_zero() const {
			// Return true if the vector is close to zero in all dimensions.
			const auto s = 1e-8;
			return (fabs(this->x) < s) && (fabs(this->y) < s) && (fabs(this->z) < s);
		}

		Vector3<T> Normalize()const {
			return (*this) / Length();
		}
	};

	template<typename T>
	inline Vector3<T> operator+(const Vector3<T>& u, const Vector3<T>& v) {
		return Vector3<T>(u.x + v.x, u.y + v.y, u.z + v.z);
	}

	template<typename T>
	inline Vector3<T> operator-(const Vector3<T>& u, const Vector3<T>& v) {
		return Vector3<T>(u.x - v.x, u.y - v.y, u.z - v.z);
	}

	template<typename T>
	inline Vector3<T> operator*(const Vector3<T>& u, const Vector3<T>& v) {
		return Vector3<T>(u.x * v.x, u.y * v.y, u.z * v.z);
	}

	template<typename T1, typename T2>
	inline Vector3<T1> operator*(T2 t, const Vector3<T1>& v) {
		return Vector3<T1>(t * v.x, t * v.y, t * v.z);
	}

	template<typename T1, typename T2>
	inline Vector3<T1> operator*(const Vector3<T1>& v, T2 t) {
		return t * v;
	}

	template<typename T1, typename T2>
	inline Vector3<T1> operator/(const Vector3<T1>& v, T2 t) {
		assert(t != 0);
		return (1 / t) * v;
	}

	template<typename T>
	inline bool operator<(const Vector3<T>& u, const Vector3<T>& v) {
		return u.x < v.x&& u.y < v.y&& u.z < v.z;
	}

	template<typename T>
	inline double Dot(const Vector3<T>& u, const Vector3<T>& v) {
		return u.x * v.x
			+ u.y * v.y
			+ u.z * v.z;
	}

	template<typename T>
	inline double AbsDot(const Vector3<T>& u, const Vector3<T>& v) {
		return std::abs(Dot(u, v));
	}

	template<typename T>
	inline Vector3<double> Cross(const Vector3<T>& u, const Vector3<T>& v) {
		return Vector3<double>(u.y * v.z - u.z * v.y,
			u.z * v.x - u.x * v.z,
			u.x * v.y - u.y * v.x);
	}

	template <typename T>
	inline T MinComponent(const Vector3<T>& v) {
		return std::min(v.x, std::min(v.y, v.z));
	}

	template <typename T>
	inline T MaxComponent(const Vector3<T>& v) {
		return std::max(v.x, std::max(v.y, v.z));
	}

	template <typename T>
	inline int	MaxDimension(const Vector3<T>& v) {
		return (v.x > v.y) ? ((v.x > v.z) ? 0 : 2) :
			((v.y > v.z) ? 1 : 2);
	}

	template<typename T>
	inline int MinDimension(const Vector3<T>& v) {
		return (v.x < v.y) ? ((v.x < v.z) ? 0 : 2) :
			((v.y < v.z) ? 1 : 2);
	}

	template <typename T>
	inline Vector3<T> Min(const Vector3<T>& p1, const Vector3<T>& p2) {
		return Vector3<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
	}

	template <typename T>
	inline Vector3<T> Max(const Vector3<T>& p1, const Vector3<T>& p2) {
		return Vector3<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
	}

	template<typename T>
	inline Vector3<T> Permute(const Vector3<T>& v, int x, int y, int z) {
		return Vector3<T>(v[x], v[y], v[z]);
	}

	template<typename T>
	inline Vector3<T> Abs(const Vector3<T>& v) {
		return Vector3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
	}

	inline Radian spherical_theta(const Vector3d& v)
	{
		return std::acos(Clamp(v.z, -1., 1.));
	}

	inline Radian spherical_phi(const Vector3d& v)
	{
		double phi = std::atan2(v.y, v.x);
		return (phi < 0) ? (phi + 2 * Pi) : phi;
	}

	inline Vector3d spherical_to_direction(double sin_theta, double cos_theta, double phi)
	{
		return Vector3d(
			sin_theta * std::cos(phi),
			sin_theta * std::sin(phi),
			cos_theta);
	}
	inline Vector3d spherical_to_direction(
		double sin_theta, double cos_theta, double phi,
		const Vector3d& x, const Vector3d& y, const Vector3d& z)
	{
		return
			sin_theta * std::cos(phi) * x +
			sin_theta * std::sin(phi) * y +
			cos_theta * z;
	}
}