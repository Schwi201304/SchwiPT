#pragma once

#include<core/schwi.h>
#include<math/point3.h>

namespace schwi {
	class Ray {
	public:
		const Point3d o;
		const Vector3d d;
		double t;

	public:
		Ray(const Point3d& o, const Vector3d& d, const double t = Infinity)
			:o(o), d(d), t(t) {	}

		Point3d operator()(double _t)const{
			return o + _t * d;
		}
	};
}