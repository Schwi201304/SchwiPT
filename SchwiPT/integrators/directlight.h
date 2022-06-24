#pragma once

#include<integrators/integrator.h>
#include<filter/gaussian.h>

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
				return scene.SkyBox(ray.direction());
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
				return scene.SkyBox(ray.direction());
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

			isect.NoL = Dot(isect.normal, (ls.position - isect.position).Normalize());

			BsdfUPtr bsdf = isect.primitive->material->Scattering(isect);
			//std::cout << isect.NoL << std::endl;
			Color f = bsdf->F(isect.wo, ls.wi);
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
		ScreenSpaceSSSIntegrator() {}

		void Render(Scene& scene, Camera& camera, Sampler& originSampler, Film& film) override {
			Film origin = ParentRender(scene, camera, originSampler, film.resolution);
			Film mask = GetMask(scene, camera, film.resolution);
			film = origin;
			for (int i = 0; i < 4; i++) {
				int r = 1 << i;
				Film g(film.resolution, std::make_unique<GaussianFilter>(Vector2d(r, r), 1), "g.png");
				g = film;
				film = film + g.Filter() * mask * (.5 / r);
			}
			//Film g1 = origin.Filter();
			//std::cout << g1.resolution << origin.resolution;
			//std::cout << (g1.resolution != origin.resolution);
			//film = origin+mask*g1;
		}

		Film ParentRender(Scene& scene, Camera& camera, Sampler& originSampler, Point2i& resolution) {
			Film film(resolution, std::make_unique<GaussianFilter>(Vector2d(1, 1), 1), "origin.png");
			auto [width, height] = resolution;
			printf("Screen Space Blur\n");
			printf("SPP:%d,Resolution:%d*%d\n", originSampler.GetSPP(), width, height);
			int finished = 0;
#ifndef _DEBUG
#pragma omp parallel for schedule(dynamic,1)
#endif
			for (int y = 0; y < height; y++) {
				std::unique_ptr<Sampler> sampler = originSampler.Clone();
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
					film.GetPixel(x, y) = L;
				}
				finished++;
			}
			fprintf(stderr, "\rRendering 100.00%%\n");
			return film;
		}

		Color Li(Ray ray, Scene& scene, Sampler& sampler)override {
			SurfaceIntersection isect;
			if (!scene.Intersect(ray, &isect)) {
				return scene.SkyBox(ray.direction());
			}

			Color Lo = isect.Le();

			if (!isect.bsdf()->IsDelta()) {
				Lo += SampleAllLight(isect, &scene, sampler, true, DirectSampleEnum::BothMis);
			}

			return Lo;
		}

		Film GetMask(Scene& scene, Camera& camera, Point2i& resolution) {
			auto [width, height] = resolution;
			Film mask(resolution, std::make_unique<GaussianFilter>(Vector2d(1, 1), 1), "mask.png");
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					Ray ray = camera.GenerateRay({ Point2d(x + .5,y + .5) });
					SurfaceIntersection isect;
					if (!scene.Intersect(ray, &isect)) {
						mask.GetPixel(x, y) = Color();
						continue;
					}

					if (isect.primitive->material->IsDelay()) {
						mask.GetPixel(x, y) = Color(1, 1, 1);
					}
					else {
						mask.GetPixel(x, y) = Color();
					}
				}
			}
			return mask;
		}

		static void SSSSS(Scene& scene, Camera& camera, Sampler& originSampler, Film& film) {
			auto [width, height] = film.resolution;
			Film mask(film.resolution, std::make_unique<GaussianFilter>(Vector2d(1, 1), 1), "origin.png");
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					Ray ray = camera.GenerateRay({ Point2d(x + .5,y + .5) });
					SurfaceIntersection isect;
					if (!scene.Intersect(ray, &isect)) {
						mask.GetPixel(x, y) = Color();
						continue;
					}

					if (isect.primitive->material->IsDelay()) {
						mask.GetPixel(x, y) = Color(1, 1, 1);
					}
					else {
						mask.GetPixel(x, y) = Color();
					}
				}
			}
			for (int i = 0; i < 4; i++) {
				int r = 1 << i;
				Film g(film.resolution, std::make_unique<GaussianFilter>(Vector2d(r, r), 1), "g.png");
				g = film;
				film = film + g.Filter() * mask * (.5 / r);
			}
			//film = mask;
			//film = film.Filter();
		}
	};
}