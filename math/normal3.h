#ifndef SCHWI_NORMAL3_H
#define SCHWI_NORMAL3_H

#include "vector3.h"
namespace schwi {
	template<typename T>
	class Normal3 :public vec3<T> {
	public:
		Normal3() :vec2() {}
		Normal3(T _x, T _y, T _z) :vec3<T>(_x, _y, _z) {}
		explicit Normal3(const Vector3<T>& v) :vec3<T>(v.x, v.y, v.z) {}

		Normal3<T> operator-() const { return Normal3<T>(-this->x, -this->y, -this->z); }
	};


	template <typename T>
	inline double dot(const Normal3<T>& n, const Vector3<T>& v) {
		return n.x * v.x + n.y * v.y + n.z * v.z;
	}

	template <typename T>
	inline Normal3<T> Faceforward(const Normal3<T>& n, const Vector3<T>& v) {
		return (Dot(n, v) < 0.) ? -n : n;
	}

	using Normal3d = Normal3<double>;
}


#endif
