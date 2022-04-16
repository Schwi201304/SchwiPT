#pragma once

#include "vec.h"

namespace schwi {
	template<typename T>
	class Vector2 :public vec2<T> {
	public:
		Vector2() :vec2<T>() {};
		Vector2(T _x, T _y) :vec2<T>(_x, _y) {}
		explicit Vector2(const vec2<T>& v) :vec2<T>(v) {}

		Vector2<T> operator-() const { return Vector2<T>(-this->x, -this->y); }

		Vector2<T>& operator+=(const Vector2<T>& v) {
			this->x += v.x;
			this->y += v.y;
			return *this;
		}

		template<typename T1>
		Vector2<T>& operator*=(const T1 t) {
			this->x *= t;
			this->y *= t;
			return *this;
		}

		template<typename T1>
		Vector2<T>& operator/=(const T1 t) {
			assert(t != 0);
			return *this *= 1 / t;
		}

		double Length() const {
			return std::sqrt(LengthSquared());
		}

		double LengthSquared() const {
			return this->x * this->x + this->y * this->y;
		}

		bool near_zero() const {
			// Return true if the vector is close to zero in all dimensions.
			const auto s = 1e-8;
			return (fabs(this->x) < s) && (fabs(this->y) < s);
		}

		Vector2<T> Normalized() {
			double invL = 1 / Length();
			x *= invL;
			y *= invL;
			return *this;
		}

		template<typename T>
		inline Vector2<double> Normalize() {
			return (*this)/Length();
		}
	};

	template<typename T>
	inline Vector2<T> operator+(const Vector2<T>& u, const Vector2<T>& v) {
		return Vector2<T>(u.x + v.x, u.y + v.y);
	}

	template<typename T>
	inline Vector2<T> operator-(const Vector2<T>& u, const Vector2<T>& v) {
		return Vector2<T>(u.x - v.x, u.y - v.y);
	}

	template<typename T>
	inline Vector2<T> operator*(const Vector2<T>& u, const Vector2<T>& v) {
		return Vector2<T>(u.x * v.x, u.y * v.y);
	}

	template<typename T1, typename T2>
	inline Vector2<T1> operator*(T2 t, const Vector2<T1>& v) {
		return Vector2<T1>(t * v.x, t * v.y);
	}

	template<typename T1, typename T2>
	inline Vector2<T1> operator*(const Vector2<T1>& v, T2 t) {
		return t * v;
	}

	template<typename T1, typename T2>
	inline Vector2<T1> operator/(const Vector2<T1>& v, T2 t) {
		assert(t != 0);
		return (1 / t) * v;
	}

	template<typename T>
	inline double Dot(const Vector2<T>& u,const Vector2<T>& v) {
		return u.x * v.x + u.y * v.y;
	}

	template <typename T>
	inline T MinComponent(const Vector2<T>& v) {
		return std::min(v.x, v.y);
	}

	template <typename T>
	inline T MaxComponent(const Vector2<T>& v) {
		return std::max(v.x, v.y);
	}

	template <typename T>
	inline int	MaxDimension(const Vector2<T>& v) {
		return (v.x > v.y) ? 0 : 1;
	}

	template<typename T>
	inline int MinDimension(const Vector2<T>& v) {
		return (v.x < v.y) ? 0 : 1;
	}

	template <typename T>
	inline Vector2<T> Min(const Vector2<T>& p1, const Vector2<T>& p2) {
		return Vector2<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y));
	}

	template <typename T>
	inline Vector2<T> Max(const Vector2<T>& p1, const Vector2<T>& p2) {
		return Vector2<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y));
	}

	template<typename T>
	inline Vector2<T> Permute(const Vector2<T>& v, int x, int y) {
		return Vector2<T>(v[x], v[y]);
	}
}