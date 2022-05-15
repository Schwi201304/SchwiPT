#pragma once

#include<core/schwi.h>
#include<core/color.h>
#include<core/ray.h>

namespace schwi {
	class Intersection {
	public:
		Point3d position{};
		Normal3d normal{};
		Vector3d wo{};
		Point2d uv{};
		double depth{};

	private:
		Color emission{};
		BsdfUPtr bsdfPtr{};
		//Surface* surface;

		//friend Surface;
		friend Primitive;

	public:
		Intersection() = default;
		Intersection(Point3d p, Normal3d n, Vector3d wo, Point2d uv = Point2d{ .5,.5 })
			:position(p), normal(n), wo(wo), uv(uv) {}

		//const Surface* surface()const { return surface; }
		const BSDF* bsdf() const { return bsdfPtr.get(); }
		Color Le()const { return emission; }

		Ray GenerateRay(const Vector3d& direction)const {
			return Ray(position, direction);
		}

		Ray GenerateRay(const Point3d& target)const {
			return Ray(position, (target - position).Normalize());
		}

		Ray GenerateRay(const Intersection& isect)const {
			return Ray(position, (isect.position - position).Normalize());
		}

	};
}