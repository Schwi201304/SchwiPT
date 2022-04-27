#pragma once

#ifndef NDEBUG
#define NDEBUG
#endif

#include"math.h"
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
		template <typename U>
		vec3(const vec3<U>& v) :x((T)v.x), y((T)v.y), z((T)v.z) {}
		template <typename U>
		vec3(const vec2<U>& v, U _z) :x((T)v.x), y((T)v.y), z((T)_z) {}
		template <typename U>
		vec3(const vec4<U>& v) :x((T)v.x), y((T)v.y), z((T)v.z) {}

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
		return out << "[" << v.x << ',' << v.y << ',' << v.z << "]";
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
		template <typename U>
		vec2(const vec2<U>& v) : x((T)v.x), y((T)v.y) {}

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
		return out << "[" << v.x << ',' << v.y << "]";
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
		template <typename U>
		vec4(const vec4<U>& v) : x((T)v.x), y((T)v.y), z((T)v.z), w((T)v.w) {}
		template <typename U>
		vec4(const vec3<U>& v, U _w) : x((T)v.x), y((T)v.y), z((T)v.z), w((T)_w) {}

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
		return out << "[" << v.x << ',' << v.y << ',' << v.z << ',' << v.w << "]";
	}

}