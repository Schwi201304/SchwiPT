#pragma once

#include<integrators/integrator.h>

namespace schwi {
	class DirectLightIntegrator :public Integrator {
	private:
		DirectSampleEnum direct_sample_enum;

	public:
		DirectLightIntegrator(DirectSampleEnum direct_sample_enum)
			:direct_sample_enum(direct_sample_enum) {}

		Color Li(Ray ray, Scene& scene, Sampler& sampler) override {
			SurfaceIntersection isect;
			if (!scene.Intersect(ray, &isect)) {
				return Color();
			}

			Color Lo = isect.Le();

			if (!isect.bsdf()->IsDelta()) {
				Lo += SampleAllLight(isect, &scene, sampler, true, direct_sample_enum);
			}

			return Lo;
		}
	};

	class SkinIntegrator :public Integrator {
	public:
		SkinIntegrator() {}

		Color Li(Ray ray, Scene& scene, Sampler& sampler)override {
			SurfaceIntersection isect;
			if (!scene.Intersect(ray, &isect)) {
				return Color();
			}

			Color Lo = isect.Le();

			if (!isect.bsdf()->IsDelta()) {
				Lo += SkinEvaluateAllLight(isect, scene, sampler);

			}

			return Lo;
		}

	private:
		Color SkinEvaluateAllLight(SurfaceIntersection& isect, Scene& scene,
			Sampler& sampler) {
			Color Ld{};

			for (const auto& light : scene.lightList) {
				Ld += SkinEvaluate(isect, *light, scene, sampler);
			}

			return Ld;
		}

		Color SkinEvaluate(SurfaceIntersection& isect, const Light& light,
			Scene& scene, Sampler& sampler) {
			Color Ld{};

			LightLiSample ls = light.SampleLi(isect, sampler.GetVector2d());
			if (ls.Li.IsBlack() || ls.pdf == 0)
				return Ld;

			isect.NoL= Dot(isect.normal, (ls.position - isect.position).Normalize());

			BsdfUPtr bsdf = isect.primitive->material->Scattering(isect);
			//std::cout << isect.NoL << std::endl;
			Color f = bsdf->f(isect.wo, ls.wi);
			if (!f.IsBlack()) {
				double cosTheta = AbsDot(ls.wi, isect.normal);
				Color dL = f * ls.Li * cosTheta / ls.pdf;
				Ld += dL;
			}

			return Ld;
		}
	};

	class ScreenSpaceSSSIntegrator :public Integrator {
	public:
		ScreenSpaceSSSIntegrator(){}

		void Render(Scene& scene, Camera& camera, Sampler& sampler, Film& film) override{
			auto [width, height] = film.resolution;
		}

		Color Li(Ray ray, Scene& scene, Sampler& sampler)override {
			return Color();
		}
	};
}