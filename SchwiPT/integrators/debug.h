#pragma once

#include<integrators/integrator.h>
#include<core/scene.h>

namespace schwi {
	class DebugIntegrator :public Integrator {
	private:
		int depth;

	public:
		DebugIntegrator(int depth):depth(depth){
			fprintf(stderr, "Debug Integrator #  depth:%d\n", depth);
		}

		Color Li(Ray ray, Scene& scene, Sampler& sampler, int depth) {
			SurfaceIntersection isect;

			if (!scene.Intersect(ray, &isect))
				return Color();
			auto bs = isect.bsdf()->Sample_f(isect.wo, sampler.GetVector2d());
			if (bs.f.IsBlack() || bs.pdf == 0.) {
				return isect.Le();
			}
			if (depth == depth) {
				//return isect.Le();
				//return { (ray.distance() - 100.) / 200.,.0,.0 };
				return bs.f;
			}

			auto uv = isect.uv;
			//return { uv[0],uv[1],0 };
			//return { (ray.distance()-100.) / 200.,.0,.0 };
			//return bs.f;
			//return { .25,.45,.65 };
			Ray wi(isect.position, bs.wi);
			return Li(wi, scene, sampler, depth + 1);
		}

		Color Li(Ray ray, Scene& scene, Sampler& sampler)override {
			return Li(ray, scene, sampler, 1);
		}
	};
}