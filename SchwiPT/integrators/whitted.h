#pragma once

#include<integrators/integrator.h>
#include<core/scene.h>

namespace schwi {
	class WhittedIntegrator :public Integrator {
	private:
		int maxDepth;

	public:
		WhittedIntegrator(int maxDepth):maxDepth(maxDepth){}

		Color Li(Ray ray, Scene& scene, Sampler& sampler, int depth) {
			Intersection isect;

			if (!scene.Intersect(ray, &isect))
				return Color();
			if (depth > maxDepth) {
				return isect.Le();
			}

			auto bs = isect.bsdf()->Sample_f(isect.wo, sampler.GetVector2d());
			if (bs.f.IsBlack() || bs.pdf == 0.) {
				return isect.Le();
			}

			//TODO
			auto uv = isect.uv;
			//return { uv[0],uv[1],0 };
			//return { (isect.depth-100) / 200,.0,.0 };
			return bs.f;
		}

		Color Li(Ray ray, Scene& scene, Sampler& sampler)override {
			return Li(ray, scene, sampler, 0);
		}
	};
}