#pragma once

#include<shapes/shape.h>

namespace schwi {
	class Cylinder :public Shape {
	private:
		double radius;
		double height;

	public:
		Cylinder(double radius, double height, const Frame* frame) :
			radius(radius), height(height), Shape(frame) {}
		//TODO Ïà½»´æÔÚbug
		virtual bool Intersect(
			const Ray& r, Intersection* out_isect
		)const override {
			Ray ray = frame->ToLocal(r);

			Point3d o = ray.origin();
			Vector3d d = ray.direction();

			double a = 1 / (d.x * d.x + d.y + d.y);
			double b = (d.x * o.x + d.y * o.y) * a;
			double c = (o.x * o.x + o.y * o.y - radius * radius) * a;

			double discr = b * b - c;
			if (discr < 0)return false;
			double sqrt_discr = std::sqrt(discr);
			double t0 = -b - sqrt_discr;
			double t1 = -b + sqrt_discr;
			if (t0 > ray.distance() || t1 <= 0)return false;

			double t = t0;
			if (t <= 0) {
				t = t1;
				if (t > ray.distance())return false;
			}

			Point3d p = ray(t);
			if (std::abs(p.z) > height / 2) {
				if (t == t1)return false;
				t = t1;
				if (t1 > ray.distance())return false;
				p = ray(t);
				if (std::abs(p.z) > height / 2)return false;
			}
			r.set_distance(t);
			Normal3d normal = Normal3d(p.x, p.y, 0).Normalize();
			normal = Dot(normal, ray.direction()) > 0 ? -normal : normal;

			double u = (std::atan2(normal.y, normal.x) + Pi) * Inv2Pi;
			double v = std::abs(p.z) * 2 / height;

			*out_isect = Intersection(
				frame->ToWorld(p),
				frame->ToWorld(normal),
				-r.direction(),
				Point2d(u, v));
			//std::cout << t << std::endl;
			return true;
		}
	};
}