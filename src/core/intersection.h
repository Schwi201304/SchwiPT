#pragma once

#include<core/schwi.h>
#include<image/color.h>
#include<math/point3.h>
#include<math/normal3.h>
#include<core/ray.h>

namespace schwi {
	class Intersection {
	public:
		Point3d position;
		Normal3d normal;
		Vector3d wo;

	private:
		Color emission;
		BsdfUPtr bsdf;
		Surface* surface;

		friend Surface;

	public:
		Intersection() = default;
		Intersection(Point3d p, Normal3d n, Vector3d wo)
			:position(p), normal(n), wo(wo) {}

		const Surface* surface()const { return surface; }
		const BSDF* bsdf() const{ return bsdf.get(); }
		Color Le()const { return emission; }

		Ray GenerateRay(const Vector3d& direction)const {
			return Ray(position, direction);
		}

		Ray GenerateRay(const Point3d& target)const {
			return Ray(position, (target - position).Normalize());
		}

		Ray GenerateRay(const Intersection& isect)const{
			return Ray(position, (isect.position - position).Normalize());
		}
	};
}