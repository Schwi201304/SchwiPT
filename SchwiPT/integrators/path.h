#pragma once

#include<integrators/integrator.h>

namespace schwi {
	enum class LightingEnum {
		Emit = 1 << 0,
		Direct = 1 << 1,
		Indirect = 1 << 2,
		AllLighting = Emit | Direct | Indirect,

		Diffuse = 1 << 3,
		Specular = 1 << 4,
		AllScattering = Diffuse | Specular,

		All = AllLighting | AllScattering
	};

	inline bool operator&(LightingEnum l1, LightingEnum l2) {
		return (int)l1 & (int)l2;
	}

	class PathIntegrator :public Integrator {
	protected:
		int maxDepth;
		DirectSampleEnum directSampleEnum;

	public:
		PathIntegrator(int maxDepth, DirectSampleEnum direct_sample_enum) :
			maxDepth(maxDepth), directSampleEnum(direct_sample_enum) {}
	};

	class SimplePathTracingRecursion :public PathIntegrator {
	public:
		using PathIntegrator::PathIntegrator;

		Color Li(Ray ray, Scene& scene, Sampler& sampler)override {
			return Li(ray, scene, sampler, 0);
		}

		Color Li(Ray ray, Scene& scene, Sampler& sampler, int depth) {
			SurfaceIntersection isect;
			if (!scene.Intersect(ray, &isect)) {
				return Color();
			}

			if (isect.primitive->areaLight)
				return isect.Le();

			if (depth >= maxDepth) {
				return isect.Le();
			}

			Vector3d wi;
			double pdf = 0;
			Color f = isect.bsdf()->Sample_f(isect.wo, &wi, &pdf, sampler.GetVector2d());
			if (f.IsBlack() || pdf == 0.) {
				return isect.Le();
			}
			//Russian Roulette
			if (++depth > 5) {
				double maxComponent = MaxComponent(ToV3d(f));
				if (sampler.GetDouble() < maxComponent) {
					f = f / maxComponent;
				}
				else {
					return isect.Le();
				}
			}

			Ray ray_wi(isect.position, wi);
			return isect.Le() +
				f * Li(ray_wi, scene, sampler, depth) * abs(Dot(wi, isect.normal)) / pdf;
		}
	};

	class PathTracingRecursion :public PathIntegrator {
	private:
		LightingEnum _lightingEnum;

	public:
		PathTracingRecursion(int maxDepth, DirectSampleEnum directSampleEnum, LightingEnum lightingEnum) :
			PathIntegrator(maxDepth, directSampleEnum),
			_lightingEnum(lightingEnum) {}

		Color Li(Ray ray, Scene& scene, Sampler& sampler)override {
			return Li(ray, scene, sampler, 0, false, _lightingEnum);
		}

	private:
		Color Li(Ray ray, Scene& scene, Sampler& sampler, int depth, bool is_last_specular, LightingEnum lightingEnum) {
			Color Lo{};
			SurfaceIntersection isect;
			bool hit = scene.Intersect(ray, &isect);

			Color Le = EmissionLighting(ray, scene, sampler, depth, is_last_specular, isect, hit);

			if (!hit || (depth >= maxDepth)) {
				if (lightingEnum & LightingEnum::Emit) {
					Lo += Le;
				}
			}
			else {
				if (lightingEnum & LightingEnum::Emit)
					Lo += Le;
				if (lightingEnum & LightingEnum::Direct)
					Lo += DirectLighting(ray, scene, sampler, depth, is_last_specular, isect);
				if (lightingEnum & LightingEnum::Indirect)
					Lo += IndirectLighting(ray, scene, sampler, depth, is_last_specular, isect, LightingEnum::All);
			}
			return Lo;
		}

		Color EmissionLighting(
			Ray ray, Scene& scene, Sampler& sampler, int depth, bool is_last_specular,
			const SurfaceIntersection& isect, bool hit) {
			Color Le{};
			if (depth == 0 || is_last_specular) {
				if (hit) {
					Le = isect.Le();
				}
				else {
					//TODO »·¾³¹â
				}
			}
			return Le;
		}

		Color DirectLighting(
			Ray ray, Scene& scene, Sampler& sampler, int depth, bool is_last_specular,
			SurfaceIntersection& isect) {
			Color Ld{};

			if (!isect.bsdf()->IsDelta()) {
				Ld = SampleAllLight(isect, &scene, sampler, true, directSampleEnum);
			}
			return Ld;
		}

		Color IndirectLighting(
			Ray ray, Scene& scene, Sampler& sampler, int depth, bool is_last_specular,
			const SurfaceIntersection& isect, LightingEnum lightingEnum) {

			Vector3d wi;
			double pdf = 0;
			Color f = isect.bsdf()->Sample_f(isect.wo, &wi, &pdf, sampler.GetVector2d());

			if (f.IsBlack() || pdf == 0.) {
				return Color();
			}

			if (++depth > 5) {
				double maxComponent = MaxComponent(ToV3d(f));
				if (sampler.GetDouble() < maxComponent) {
					f *= (1 / maxComponent);
				}
				else {
					return isect.Le();
				}
			}

			Ray ray_wi(isect.position, wi);
			return f
				* Li(ray_wi, scene, sampler, depth, isect.bsdf()->IsDelta(), lightingEnum)
				* AbsDot(wi, isect.normal) / pdf;
		}
	};
}
