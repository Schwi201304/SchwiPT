#pragma once

#include<integrators/integrator.h>
#include<core/scene.h>

namespace schwi {
	class DebugIntegrator :public Integrator {
	private:
		int depth;

	public:
		DebugIntegrator(int depth) :depth(depth) {
			fprintf(stderr, "Debug Integrator #  depth:%d\n", depth);
		}

		Color Li(Ray ray, Scene& scene, Sampler& sampler, int depth) {
			SurfaceIntersection isect;

			if (!scene.Intersect(ray, &isect))
				return Color();

			Vector3d wi;
			double pdf = 0;
			Color f = isect.bsdf()->Sample_f(isect.wo, &wi, &pdf, sampler.GetVector2d());
			if (f.IsBlack() || pdf == 0.) {
				return isect.Le();
			}
			if (depth == depth) {
				//return isect.Le();
				//return { (ray.distance() - 100.) / 200.,.0,.0 };
				return f;
			}

			auto uv = isect.uv;
			//return { uv[0],uv[1],0 };
			//return { (ray.distance()-100.) / 200.,.0,.0 };
			//return f;
			//return { .25,.45,.65 };
			Ray ray_wi(isect.position, wi);
			return Li(ray_wi, scene, sampler, depth + 1);
		}

		Color Li(Ray ray, Scene& scene, Sampler& sampler)override {
			return Li(ray, scene, sampler, 1);
		}
	};
}