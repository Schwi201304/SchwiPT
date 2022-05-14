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

		virtual Bounds3d WorldBound() const override {
			auto position = frame->position();
			auto offset = frame->binormal() * radius + frame->tangent() * radius;
			return Bounds3d(position - offset, position + offset);
		}

		virtual double Area()const override {
			return radius * radius * Pi;
		}

		virtual Intersection SamplePosition(
			const Vector2d& random, double* pdf
		)const override {
			Intersection isect;

			Point2d sample_point = SampleDiskConcentric(random);
			isect.position = frame->position()
				+ radius * (frame->binormal() * sample_point.x
					+ frame->tangent() * sample_point.y);
			isect.normal = Normal3d(frame->normal());

			*pdf = 1 / Area();
			return std::move(isect);
		}
	};
}