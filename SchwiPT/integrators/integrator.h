#pragma once

#include<core/schwi.h>
#include<core/ray.h>
#include<math/vector2.h>
#include<core/color.h>
#include<samplers/sampler.h>
#include<cameras/camera.h>
#include<core/film.h>

namespace schwi {
	class Integrator {
	public:
		~Integrator() = default;
		Integrator() {}

		void Render(Scene& scene, Camera& camera, Sampler& originalSampler, Film& film) {
			auto [width, height] = film.resolution;

			printf("SPP:%d,Resolution:%d*%d\n", originalSampler.GetSPP(), width, height);

			int finished = 0;
#ifndef _DEBUG
#pragma omp parallel for schedule(dynamic,1)
#endif
			for (int y = 0; y < height; y++) {
				std::unique_ptr<Sampler> sampler = originalSampler.Clone();
				fprintf(stderr, "\rRendering %5.2f%%", 100. * finished / height);
				for (int x = 0; x < width; x++) {
					Color L{};
					sampler->StartPixel();
					do {
						auto cameraSample = sampler->GetCameraSample(Point2d(x, y));
						auto ray = camera.GenerateRay(cameraSample);

						L = L + Li(ray, scene, *sampler);
					} while (sampler->NextSample());
					L = L / sampler->GetSPP();
					film.GetPixel(x, y)= L;
				}
				finished++;
			}
			fprintf(stderr, "\rRendering 100.00%%\n");
		}

		virtual Color Li(Ray ray, Scene& scene, Sampler& sampler) = 0;

	protected:
		Color EstimateDirectLightDirection(const SurfaceIntersection& isect, const Light& light,
			const Vector2d& random, Scene* scene, Sampler& sampler, bool skip_specular) {
			Color Ld{};

			if (light.IsDelta())
				return Ld;
			if (skip_specular && isect.bsdf()->IsDelta())
				return Ld;

			BSDFSample bs = isect.bsdf()->Sample_f(isect.wo, sampler.GetVector2d());
			if (bs.f.IsBlack() || bs.pdf == 0)
				return Ld;

			Ray ray = isect.GenerateRay(bs.wi);
			SurfaceIntersection light_isect;
			bool is_hit = scene->Intersect(ray, &light_isect);

			Color Li;
			if (is_hit) {
				if (light_isect.primitive->areaLight == &light)
					Li = light_isect.Le();
			}
			else {
				Li = light.Le(ray);
			}

			if (!Li.IsBlack()) {
				double cosTheta = AbsDot(bs.wi, isect.normal);
				Ld = bs.f * Li * cosTheta / bs.pdf;
			}

			return Ld;
		}

		Color EstimateDirectLightPosition(const SurfaceIntersection& isect, const Light& light,
			const Vector2d& random, Scene* scene, Sampler& sampler, bool skip_specular) {
			Color Ld{};

			if (skip_specular && isect.bsdf()->IsDelta())
				return Ld;

			LightLiSample ls = light.SampleLi(isect, random);
			if (ls.Li.IsBlack() || ls.pdf == 0)
				return Ld;

			if (scene->Occluded(isect, ls.position))
				return Ld;

			Color f = isect.bsdf()->f(isect.wo, ls.wi);
			if (!f.IsBlack()) {
				double cosTheta = AbsDot(ls.wi, isect.normal);
				Color dL = f * ls.Li * cosTheta / ls.pdf;
				Ld += dL;
			}

			return Ld;
		}
	};
}