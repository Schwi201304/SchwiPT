#pragma once

#include<integrators/integrator.h>

namespace schwi {
	class DirectLightIntegrator:public Integrator {
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
}