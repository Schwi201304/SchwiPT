#pragma once

#include<core/schwi.h>
#include<core/intersection.h>
#include<core/bounds.h>
#include<samplers/sampler.h>
#include<samplers/sampling.h>
#include<core/frame.h>

namespace schwi {
	class Shape {
	public:
		const Frame* frame;

	public:
		~Shape() {
			delete frame;
		}
		Shape(const Frame* frame) :
			frame(frame) {}

		virtual bool Intersect(const Ray& ray, SurfaceIntersection* isect)const = 0;
		virtual Bounds3d WorldBound()const = 0;
		virtual double Area()const = 0;

	public:
		virtual SurfaceIntersection SamplePosition(
			const Vector2d& random, double* out_pdf_p)const = 0;

		virtual double PdfPosition(const SurfaceIntersection& isect)const {
			return 1 / Area();
		}

		virtual SurfaceIntersection SampleDirection(
			const SurfaceIntersection& isect, const Vector2d& random, double* out_pdf_p
		)const {
			SurfaceIntersection light_isect = SamplePosition(random, out_pdf_p);
			Vector3d wi = light_isect.position - isect.position;

			if (wi.LengthSquared() == 0) {
				*out_pdf_p = 0;
			}
			else {
				wi = wi.Normalize();
				*out_pdf_p *= DistanceSquared(light_isect.position, isect.position) / std::abs(Dot(light_isect.normal, -wi));
				if (std::isinf(*out_pdf_p))
					*out_pdf_p = 0;
			}

			return light_isect;
		}

		virtual double PdfDirection(
			const SurfaceIntersection& isect, const Vector3d& world_wi
		)const {
			Ray ray = isect.GenerateRay(world_wi);
			SurfaceIntersection light_isect;

			if (!Intersect(ray, &light_isect))
				return 0;

			double pdf = DistanceSquared(isect.position, light_isect.position) / (std::abs(Dot(light_isect.normal, -world_wi) * Area()));
			if (std::isinf(pdf))
				pdf = 0.;

			return pdf;
		}

	public:
		static constexpr double epsilon = 1e-3;
	};
}