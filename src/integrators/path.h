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
			Intersection isect;
			if (!scene.Intersect(ray, &isect)) {
				return Color();
			}
			if (depth > maxDepth) {
				return isect.Le();
			}
			auto bs = isect.bsdf()->Sample_f(isect.wo, sampler.GetVector2d());
			if (bs.f.IsBlack() || bs.pdf == 0.) {
				return isect.Le();
			}
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
			return isect.Le() +
				bs.f * Li(wi, scene, sampler, depth) * std::abs(Dot(bs.wi, isect.normal)) / bs.pdf;
		}

		Color Li(Ray ray, Scene& scene, Sampler& sampler)override {
			return Li(ray, scene, sampler, 0);
		}
	};
}
