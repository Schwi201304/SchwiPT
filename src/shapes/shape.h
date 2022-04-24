#pragma once

#include<core/schwi.h>
#include<core/intersection.h>
#include<math/bounds.h>
#include<samplers/sampler.h>
#include<core/frame.h>

namespace schwi {
	class Shape {
	public:
		virtual bool Intersect(const Ray& ray, Intersection* isect)const = 0;		
		virtual Bounds3d WorldBound()const = 0;
		virtual double Area()const = 0;

	public:
		virtual Intersection SamplePosition(
			const Vector2d& random, double* out_pdf_p)const = 0;

		virtual double PdfPosition(const Intersection& isect)const {
			return 1 / Area();
		}

		virtual Intersection SampleDirection(
			const Intersection& isect, const Vector2d& random, double* out_pdf_p
		)const {
			Intersection light_isect = SamplePosition(random, out_pdf_p);
			Vector3d wi = light_isect.position - isect.position;

			if (wi.LengthSquared() == 0) {
				*out_pdf_p = 0;
			}
			else {
				wi = wi.Normalize();
				*out_pdf_p *= DistanceSquared(light_isect.position, isect.position)/std::abs(Dot(light_isect.normal,-wi));
				if (std::isinf(*out_pdf_p))
					*out_pdf_p = 0;
			}

			return std::move(light_isect);
		}
		
		virtual double PdfDirection(
			const Intersection& isect, const Vector3d& world_wi
		)const {
			Ray ray = isect.GenerateRay(world_wi);
			Intersection light_isect;

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