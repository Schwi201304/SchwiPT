#pragma once

#include<shapes/shape.h>

namespace schwi {
	class Sphere :public Shape {
	private:
		Point3d center{ 0,0,0 };
		double radius, radius_sq;

	public:
		Sphere(double r, const Frame* frame)
			:radius(r), radius_sq(r* r), Shape(frame) {}

		virtual bool Intersect(
			const Ray& r, Intersection* out_isect
		)const override {
			Ray ray = frame->ToLocal(r);

			Vector3d oc = center - ray.origin();
			double neg_b = Dot(oc, ray.direction());
			double discr = neg_b * neg_b - Dot(oc, oc) + radius_sq;

			double t = 0;
			bool hit = false;
			if (discr >= 0) {
				double sqrt_discr = std::sqrt(discr);

				if (t = neg_b - sqrt_discr; t > epsilon && t < ray.distance())
					hit = true;
				else if (t = neg_b + sqrt_discr; t > epsilon && t < ray.distance())
					hit = true;
			}

			if (hit) {
				r.set_distance(t);
				Point3d hit_point = ray(t);
				Normal3d normal = Normal3d((hit_point - center).Normalize());

				double u = (std::atan2(normal.y, normal.x) + Pi) * Inv2Pi;
				double v = acos(normal.z) * InvPi;

				*out_isect = Intersection(
					frame->ToWorld(hit_point),
					frame->ToWorld(normal),
					-r.direction(),
					Point2d(u, v));
			}

			return hit;
		}

		virtual Bounds3d WorldBound() const override {
			Vector3d half(radius, radius, radius);
			return Bounds3d(center - half, center + half);
		}

		virtual double Area()const override {
			return 4 * Pi * radius_sq;
		}

	public:
		Intersection SamplePosition(
			const Vector2d& random, double* pdf
		)const override {
			Vector3d direction = SampleSphereUniform(random);
			Point3d position = center + radius * direction;

			Intersection isect;
			isect.position = position;
			isect.normal = Normal3d(direction.Normalize());

			*pdf = 1 / Area();

			return isect;
		}

		Intersection SampleDirection(
			const Intersection& isect, const Vector2d& random, double* pdf
		)const override {
			if (DistanceSquared(isect.position, center) <= radius_sq) {
				Intersection light_isect = SamplePosition(random, pdf);
				Vector3d wi = light_isect.position - isect.position;

				if (wi.LengthSquared() == 0)
					*pdf = 0;
				else {
					wi = wi.Normalize();
					*pdf *= DistanceSquared(light_isect.position, isect.position) / std::abs(Dot(isect.normal, -wi));
				}

				if (std::isinf(*pdf))
					*pdf = 0.;

				return light_isect;
			}

			double dist = Distance(isect.position, center);
			double inv_dist = 1 / dist;

			double sin_theta_max = radius * inv_dist;
			double sin_theta_max_sq = sin_theta_max * sin_theta_max;
			double inv_sin_theta_max = 1 / sin_theta_max;

			double cos_theta_max = std::sqrt(std::max(0., 1 - sin_theta_max_sq));
			double cos_theta = (cos_theta_max - 1) * random[0] + 1;
			double sin_theta_sq = 1 - cos_theta * cos_theta;

			//sin^2(1.5 deg)=0.00068523)
			if (sin_theta_max_sq < 0.00068523) {
				sin_theta_sq = sin_theta_max_sq * random[0];
				cos_theta = std::sqrt(1 - sin_theta_sq);
			}

			double cos_alpha = sin_theta_sq * inv_sin_theta_max +
				cos_theta * std::sqrt(std::max(0., 1 - sin_theta_sq * inv_sin_theta_max * inv_sin_theta_max));
			double sin_alpha = std::sqrt(std::max(0., 1 - cos_alpha * cos_alpha));
			double phi = random[1] * 2 * Pi;

			Normal3d normal = Normal3d(center - isect.position) * inv_dist;
			Frame frame(normal);

			Normal3d world_normal = Normal3d(spherical_to_direction(sin_alpha, cos_alpha, phi, -frame.binormal(), -frame.tangent(), -frame.normal()));
			Point3d world_position = center + radius * Vector3d(world_normal.x, world_normal.y, world_normal.z);

			Intersection light_isect;
			light_isect.position = world_position;
			light_isect.normal = world_normal;

			*pdf = 1 / (2 * Pi * (1 - cos_theta_max));

			return light_isect;
		}

		double PdfDirection(
			const Intersection& isect, const Vector3d& world_wi
		)const override {
			if (DistanceSquared(isect.position, center) <= radius_sq)
				return Shape::PdfDirection(isect, world_wi);

			double sin_theta_max_sq = radius_sq / DistanceSquared(isect.position, center);
			double cos_theta_max = std::sqrt(std::max(0., 1 - sin_theta_max_sq));
			return PdfConeUniform(cos_theta_max);
		}
	};
}