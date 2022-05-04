#pragma once

#include<shapes/shape.h>

namespace schwi {
	class Disk :public Shape {
	private:
		double radius;

	public:
		Disk(double radius, const Frame* frame) :
			radius(radius), Shape(frame) {}

		virtual bool Intersect(
			const Ray& r, Intersection* out_isect
		)const override {
			Ray ray = frame->ToLocal(r);
			Normal3d normal{ 0,0,1 };
			if (std::abs(Dot(ray.direction(), normal)) < epsilon)return false;

			Vector3d op = Point3d(0, 0, 0) - ray.origin();
			double t = Dot(normal, op) / Dot(normal, ray.direction());

			if ((t > epsilon) && (t < ray.distance())) {
				Point3d hit_point = ray(t);
				if (Distance({ 0,0,0 }, hit_point) <= radius) {
					r.set_distance(t);
					*out_isect = Intersection(
						frame->ToWorld(hit_point),
						Normal3d(frame->normal()),
						-r.direction(),
						Point2d(.5, .5));
					return true;
				}
			}
			return false;
		}
	};
}