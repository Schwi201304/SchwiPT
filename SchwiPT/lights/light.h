#pragma once

#include<core/schwi.h>
#include<core/intersection.h>
#include<core/color.h>

namespace schwi {
	enum class LightEnum {
		Position = 1 << 0,
		Direction = 1 << 1,
		Area = 1 << 2,
		Infinite = 1 << 3,
	};

	inline bool IsDeltaLight(int flags) {
		return flags & (int)LightEnum::Position ||
			flags & (int)LightEnum::Direction;
	}

	struct LightLeSample {
		bool is_delta;
		Point3d position;
		Normal3d normal;
		Ray ray;
		double pdf_position = 0;
		double pdf_direction = 0;
		Color Le;

		LightLeSample() = default;
		LightLeSample(const Point3d& position, const Normal3d& normal, const Ray& ray,
			double pdf_position, double pdf_direction, const Color& emission) :
			position(position), normal(normal), ray(ray),
			pdf_position(pdf_position), pdf_direction(pdf_direction), Le(emission) {}

		double AbsCosTheta(Vector3d w) {
			return is_delta ? 1 : std::abs(Dot(w, normal));
		}
	};

	struct LightLiSample {
		Point3d position;
		Vector3d wi;
		double pdf{};
		Color Li;

		LightLiSample() = default;
		LightLiSample(const Intersection& isect, const Vector3d& wi,
			double pdf, const Color& Li) :
			position(isect.position), wi(wi), pdf(pdf), Li(Li) {}
	};

	class Light {
	protected:
		Point3d world_position;
		int samples_num;

	public:
		Light(const Point3d& world_position, int samples_num = 1) :
			world_position(world_position), samples_num(samples_num) {}

		virtual bool IsDelta()const = 0;
		virtual bool IsFinite()const = 0;
		virtual void Preprocess(const Scene& scene) {};
		virtual Color Power()const = 0;

		virtual Color Le(const Ray& ray)const {
			return Color();
		}

		virtual Color SampleLe(const Vector2d& random1, const Vector2d& random2,
			Ray* ray, Normal3d* light_normal,
			double* pdf_position, double* pdf_direction)const = 0;

		virtual void PdfLe(const Ray& ray, const Normal3d& light_normal,
			double* pdf_position, double* pdf_direction)const = 0;

		virtual LightLiSample SampleLi(const Intersection& isect, const Vector2d& random)const = 0;
		virtual double PdfLi(const Intersection& isect, const Vector3d& world_wi)const = 0;
	};

	class PointLight :public Light {
	private:
		Color intensity;

	public:
		PointLight(const Point3d& world_position,int samples_num,Color intensity):
			Light(world_position,samples_num),
			intensity(intensity){}

		virtual bool IsDelta() const override { return true; }
		virtual bool IsFinite() const override { return true; }

		virtual Color Power() const override { return 4 * Pi * intensity; }
	};

	class AreaLight :public Light {
	private:
		Color radiance;
		Color power;
		const Shape* shape;

	public:
		AreaLight(const Point3d& world_position, int samples_num,
			const Color& radiance, const Shape* shape) :
			Light(world_position, samples_num),
			radiance(radiance), shape(shape),
			power(radiance* shape->Area()* Pi) {}

		virtual bool IsDelta()const override { return false; }
		virtual bool IsFinite()const override { return true; }
		virtual Color Power()const override { return power; }

		Color Le(const Intersection& light_isect, const Vector3d& wo)const {
			return (Dot(light_isect.normal, wo) > 0) ?
				radiance :
				Color();
		}

		virtual Color SampleLe(
			const Vector2d& random1, const Vector2d& random2,
			Ray* ray, Normal3d* light_normal,
			double* pdf_position, double* pdf_direction
		)const override {
			Intersection light_isect = shape->SamplePosition(random1, pdf_position);
			*light_normal = light_isect.normal;

			Vector3d w = SampleHemisphereCosine(random2);
			*pdf_direction = PdfHemisphereCosine(w.z);

			Frame frame{ light_isect.normal };
			Vector3d w2 = w.x * frame.binormal() + w.y * frame.tangent() + w.z * frame.normal();
			*ray = light_isect.GenerateRay(w2);

			return Le(light_isect, w2);
		}

		virtual void PdfLe(
			const Ray& ray, const Normal3d& light_normal,
			double* pdf_position, double* pdf_direction
		)const override {
			Intersection isect(ray.origin(), light_normal, Vector3d());
			*pdf_position = shape->PdfPosition(isect);
			*pdf_direction = PdfHemisphereCosine(Dot(light_normal, ray.direction()));
		}

		virtual LightLiSample SampleLi(
			const Intersection& isect, const Vector2d& random
		) const override {
			LightLiSample sample;
			Intersection light_isect = shape->SampleDirection(isect, random, &sample.pdf);
			sample.position = light_isect.position;

			if (sample.pdf == 0 || (light_isect.position - isect.position).LengthSquared() == 0) {
				sample.Li = Color();
			}
			else {
				sample.wi = (light_isect.position - isect.position).Normalize();
				sample.Li = Le(light_isect, -sample.wi);
			}

			return sample;
		}

		virtual double PdfLi(
			const Intersection& isect, const Vector3d& world_wi
		)const override {
			return shape->PdfDirection(isect, world_wi);
		}
	};
}