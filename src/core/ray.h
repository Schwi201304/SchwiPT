#pragma once

#include<core/schwi.h>
#include<math/point3.h>

namespace schwi {
	class Ray {
	private:
		Point3d o;
		Vector3d d;
		mutable double t;

	public:
		Ray(const Point3d& o, const Vector3d& d, const double t = Infinity)
			:o(o), d(d), t(t) {	}

		Point3d operator()(double _t)const {
			return o + _t * d;
		}

		Point3d origin() const{ return o; }
		Vector3d direction() const{ return d; }
		double distance()const { return t; }
		void set_distance(double _t) const{ t = _t; }
	};
}