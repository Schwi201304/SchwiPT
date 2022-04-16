#pragma once

#include "vector2.h"
namespace schwi {
	template<typename T>
	class Point2 :public vec2<T> {
	public:
		Point2() :vec2<T>() {}
		Point2(T _x, T _y) :vec2<T>(_x, _y) {}
		explicit Point2(const vec2<T>& v) :vec2<T>(v) {}

		template<typename U>
		explicit Point2(const Point2<U>& p) :vec2((T)p.x, (T)p.y) {}

		template<typename U>
		explicit operator Vector2<U>() const {
			return Vector2<U>(this->x, this->y);
		}

		Point2<T> operator+(const Vector2<T>& v) const {
			return Point2<T>(this->x + v.x, this->y + v.y);
		}

		Point2<T>& operator+=(const Vector2<T>& v) {
			this->x += v.x; this->y += v.y;
			return *this;
		}

		Vector2<T> operator-(const Point2<T>& p) const {
			return Vector2<T>(this->x - p.x, this->y - p.y);
		}

		Point2<T> operator-(const Vector2<T>& v) const {
			return Point2<T>(this->x - v.x, this->y - v.y);
		}

		Point2<T>& operator-=(const Vector2<T>& v) {
			this->x -= v.x; this->y -= v.y;
			return *this;
		}
	};

	template <typename T>
	inline double Distance(const Point2<T>& p1, const Point2<T>& p2) {
		return (p1 - p2).Length();
	}

	template <typename T>
	inline double DistanceSquared(const Point2<T>& p1, const Point2<T>& p2) {
		return (p1 - p2).LengthSquared();
	}

	template<typename T>
	inline Point2<T> operator+(const Point2<T>& u, const Point2<T>& v) {
		return Point2<T>(u.x + v.x, u.y + v.y);
	}

	template <typename T>
	Point2<T> Lerp(double t, const Point2<T>& p0, const Point2<T>& p1) {
		return Point2<T>(
			(1 - t) * p0.x + t * p1.x,
			(1 - t) * p0.y + t * p1.y);
	}

	template <typename T>
	Point2<T> Floor(const Point2<T>& p) {
		return Point2<T>(std::floor(p.x), std::floor(p.y));
	}
	template <typename T>
	Point2<T> Ceil(const Point2<T>& p) {
		return Point2<T>(std::ceil(p.x), std::ceil(p.y));
	}
	template <typename T>
	Point2<T> Abs(const Point2<T>& p) {
		return Point2<T>(std::abs(p.x), std::abs(p.y));
	}

	template <typename T>
	inline Point2<T> Min(const Point2<T>& p1, const Point2<T>& p2) {
		return Point2<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y));
	}

	template <typename T>
	inline Point2<T> Max(const Point2<T>& p1, const Point2<T>& p2) {
		return Point2<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y));
	}

	template<typename T>
	inline Point2<T> Permute(const Point2<T>& v, int x, int y) {
		return Point2<T>(v[x], v[y]);
	}

	using Point2d = Point2<double>;
	using Point2i = Point2<int>;
}
