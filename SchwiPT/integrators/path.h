#pragma once

#include<core/scene.h>
#include<integrators/integrator.h>

namespace schwi {
	class PathIntegrator :public Integrator {
	private:
		int maxDepth;

	public:
		PathIntegrator(int maxDepth) :
			maxDepth(maxDepth) {}

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
			
			BSDFSample bs = isect.bsdf()->Sample_f(isect.wo, sampler.GetVector2d());
			if (bs.f.IsBlack() || bs.pdf == 0.) {
				return isect.Le();
			}
			//Russian Roulette
			if (++depth > 5) {
				auto maxComponent = MaxComponent(ToV3d(bs.f));
				if (sampler.GetDouble() < maxComponent) {
					bs.f = bs.f / maxComponent;
				}
				else {
					return isect.Le();
				}
			}

			Ray wi(isect.position, bs.wi);
			return EstimateDirectLightPosition(isect, *scene.lightList[0], sampler.GetVector2d(), &scene, sampler, true);
			return isect.Le() +
				bs.f * Li(wi, scene, sampler, depth) * abs(Dot(bs.wi, isect.normal)) / bs.pdf;
		}

		Color Li(Ray ray, Scene& scene, Sampler& sampler)override {
			return Li(ray, scene, sampler, 0);
		}
	};
}
