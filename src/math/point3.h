#pragma once

#include "vector3.h"
namespace schwi {
	template<typename T>
	class Point3 :public vec3<T> {
	public:
		Point3() :vec3<T>() {}
		Point3(T _x, T _y, T _z) :vec3<T>(_x, _y, _z) {}
		explicit Point3(const vec3<T>& v):vec3<T>(v){}

		template<typename U>
		explicit Point3(const Point3<U>& p) :vec2((T)p.x, (T)p.y, (T)p.z) {}

		template<typename U>
		explicit operator Vector3<U>() const{
			return Vector3<U>(this->x, this->y, this->z);
		}

		Point3<T> operator+(const Vector3<T>& v) const {
			return Point3<T>(this->x + v.x, this->y + v.y, this->z + v.z);
		}

		Point3<T>& operator+=(const Vector3<T>& v) {
			this->x += v.x; this->y += v.y; this->z += v.z;
			return *this;
		}

		Vector3<T> operator-(const Point3<T>& p) const {
			return Vector3<T>(this->x - p.x, this->y - p.y, this->z - p.z);
		}

		Point3<T> operator-(const Vector3<T>& v) const {
			return Point3<T>(this->x - v.x, this->y - v.y, this->z - v.z);
		}

		Point3<T>& operator-=(const Vector3<T>& v) {
			this->x -= v.x; this->y -= v.y; this->z -= v.z;
			return *this;
		}
	};

	template <typename T>
	inline double Distance(const Point3<T>& p1, const Point3<T>& p2) {
		return (p1 - p2).Length();
	}

	template <typename T>
	inline double DistanceSquared(const Point3<T>& p1, const Point3<T>& p2) {
		return (p1 - p2).LengthSquared();
	}

	template<typename T>
	inline Point3<T> operator+(const Point3<T>& u, const Point3<T>& v) {
		return Point3<T>(u.x + v.x, u.y + v.y, u.z + v.z);
	}

	template<typename T1, typename T2>
	inline Point3<T1> operator*(T2 t, const Point3<T1>& v) {
		return Point3<T1>(t * v.x, t * v.y, t * v.z);
	}

	template<typename T1, typename T2>
	inline Point3<T1> operator*(const Point3<T1>& v, T2 t) {
		return t * v;
	}

	template <typename T>
	Point3<T> Lerp(double t, const Point3<T>& p0, const Point3<T>& p1) {
		return (1 - t) * p0 + t * p1;
	}

	template <typename T>
	Point3<T> Floor(const Point3<T>& p) {
		return Point3<T>(std::floor(p.x), std::floor(p.y), std::floor(p.z));
	}
	template <typename T>
	Point3<T> Ceil(const Point3<T>& p) {
		return Point3<T>(std::ceil(p.x), std::ceil(p.y), std::ceil(p.z));
	}
	template <typename T>
	Point3<T> Abs(const Point3<T>& p) {
		return Point3<T>(std::abs(p.x), std::abs(p.y), std::abs(p.z));
	}

	template <typename T>
	inline Point3<T> Min(const Point3<T>& p1, const Point3<T>& p2) {
		return Point3<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
	}

	template <typename T>
	inline Point3<T> Max(const Point3<T>& p1, const Point3<T>& p2) {
		return Point3<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
	}

	template<typename T>
	inline Point3<T> Permute(const Point3<T>& v, int x, int y, int z) {
		return Point3<T>(v[x], v[y], v[z]);
	}

	using Point3d = Point3<double>;
	using Point3i = Point3<int>;
}