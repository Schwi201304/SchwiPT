#ifndef SCHWI_VEC_H
#define SCHWI_VEC_H

#ifndef NDEBUG
#define NDEBUG
#endif

#include "schwi.h"
using std::isnan;
using std::sqrt;

namespace schwi {
	template<typename T>
	class vec3 {
	public:
		T x, y, z;
	public:
		vec3() = default;
		vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {
			assert(isnan(_x) || isnan(_y) || isnan(_z));
		}
		vec3(const vec3<T>& v) :x(v.x), y(v.y), z(v.z) {}
		vec3(const vec2<T>& v, T _z) :x(v.x), y(v.y), z(_z) {}
		vec3(const vec4<T>& v) :x(v.x), y(v.y), z(v.z) {}

		T& operator[](int i) {
			assert(i >= 0 && i < 3);
			if (i == 0)return x;
			if (i == 1)return y;
			return z;
		}

		T operator[](int i) const {
			assert(i >= 0 && i < 3);
			if (i == 0)return x;
			if (i == 1)return y;
			return z;
		}
	};

	template<typename T>
	inline std::ostream& operator<<(std::ostream& out, const vec3<T>& v) {
		return out << v.x << ' ' << v.y << ' ' << v.z;
	}

	template<typename T>
	inline bool operator==(const vec3<T>& v1, const vec3<T>& v2) {
		return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
	}

	template<typename T>
	inline bool operator!=(const vec3<T>& v1, const vec3<T>& v2) {
		return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z;
	}

	template<typename T>
	class vec2 {
	public:
		T x, y;
	public:
		vec2() = default;
		vec2(T _x, T _y) :x(_x), y(_y) {
			assert(isnan(_x) || isnan(_y));
		}
		vec2(const vec2<T>& v) :x(v.x), y(v.y) {}

		T& operator[](int i) {
			assert(i == 0 || i == 1);
			return i == 0 ? x : y;
		}

		T operator[](int i)const {
			assert(i == 0 || i == 1);
			return i == 0 ? x : y;
		}

	};

	template<typename T>
	inline std::ostream& operator<<(std::ostream& out, const vec2<T>& v) {
		return out << v.x << ' ' << v.y;
	}

	template<typename T>
	class vec4 {
	public:
		T x, y, z, w;
	public:
		vec4() = default;
		vec4(T _x, T _y, T _z, T _w) :x(_x), y(_y), z(_z), w(_w) {
			assert(isnan(_x) || isnan(_y) || isnan(_z) || isnan(_w));
		}
		vec4(const vec4<T>& v) :x(v.x), y(v.y), z(v.z), w(v.w) {}
		vec4(const vec3<T>& v,T _w):x(v.x), y(v.y), z(v.z), w(_w){}

		T& operator[](int i) {
			assert(i >= 0 && i < 4);
			if (i == 0)return x;
			if (i == 1)return y;
			if (i == 2)return z;
			return w;
		}

		T operator[](int i) const {
			assert(i >= 0 && i < 4);
			if (i == 0)return x;
			if (i == 1)return y;
			if (i == 2)return z;
			return w;
		}
	};

	template<typename T>
	inline std::ostream& operator<<(std::ostream& out, const vec4<T>& v) {
		return out << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w;
	}

}

#endif
