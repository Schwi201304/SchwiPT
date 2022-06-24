#pragma once

#include<core/schwi.h>
#include<core/ray.h>
#include<math/vector2.h>
#include<core/color.h>
#include<samplers/sampler.h>
#include<cameras/camera.h>
#include<core/film.h>
#include<core/scene.h>

namespace schwi {
	enum class DirectSampleEnum {
		SampleSingleLight = 1 << 0,
		SampleAllLight = 1 << 1,

		Bsdf = 1 << 2,
		Light = 1 << 3,

		BsdfMis = 1 << 4,
		LightMis = 1 << 5,
		BothMis = BsdfMis | LightMis,

		Default = SampleAllLight | BothMis
	};

	class Integrator {
	public:
		~Integrator() = default;
		Integrator() {}

		virtual void Render(Scene& scene, Camera& camera, Sampler& originalSampler, Film& film) {
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
					film.GetPixel(x, y) = L;
				}
				finished++;
			}
			fprintf(stderr, "\rRendering 100.00%%\n");
		}

		virtual Color Li(Ray ray, Scene& scene, Sampler& sampler) = 0;

	protected:
	};

	Color EstimateDirectLightDirection(SurfaceIntersection& isect, const Light& light,
		Scene* scene, Sampler& sampler, bool skip_specular) {
		Color Ld{};

		if (light.IsDelta())
			return Ld;
		if (skip_specular && isect.bsdf()->IsDelta())
			return Ld;

		Vector3d wi;
		double pdf = 0;
		Color f = isect.bsdf()->Sample_f(isect.wo, &wi, &pdf, sampler.GetVector2d());
		if (f.IsBlack() || pdf == 0)
			return Ld;

		Ray ray = isect.GenerateRay(wi);
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
			double cosTheta = AbsDot(wi, isect.normal);
			Ld = f * Li * cosTheta / pdf;
		}

		return Ld;
	}

	Color EstimateDirectLightPosition(SurfaceIntersection& isect, const Light& light,
		Scene* scene, Sampler& sampler, bool skip_specular) {
		Color Ld{};

		if (skip_specular && isect.bsdf()->IsDelta())
			return Ld;

		LightLiSample ls = light.SampleLi(isect, sampler.GetVector2d());
		if (ls.Li.IsBlack() || ls.pdf == 0)
			return Ld;

		if (scene->Occluded(isect, ls.position))
			return Ld;

		Color f = isect.bsdf()->F(isect.wo, ls.wi);
		if (!f.IsBlack()) {
			double cosTheta = AbsDot(ls.wi, isect.normal);
			Color dL = f * ls.Li * cosTheta / ls.pdf;
			Ld += dL;
		}

		return Ld;
	}

	Color EstimateDirectLightDirectionMis(SurfaceIntersection& isect, const Light& light,
		Scene* scene, Sampler& sampler, bool skip_specular) {
		Color Ld{};

		bool is_specular = isect.bsdf()->IsDelta();
		if (is_specular && skip_specular)
			return Ld;

		if (light.IsDelta())
			return Ld;

		Vector3d wi;
		double pdf = 0;
		Color f = isect.bsdf()->Sample_f(isect.wo, &wi, &pdf, sampler.GetVector2d());
		f *= AbsDot(wi, isect.normal);

		if (!f.IsBlack() && pdf > 0) {
			Ray ray = isect.GenerateRay(wi);
			SurfaceIntersection light_isect;
			bool is_hit = scene->Intersect(ray, &light_isect);

			Color Li;
			if (is_hit) {
				if (light_isect.primitive->areaLight == &light) {
					Li = light_isect.Le();
				}
			}
			else {
				Li = light.Le(ray);
			}

			if (!Li.IsBlack()) {
				if (is_specular) {
					Ld += f * Li / pdf;
				}
				else {
					double light_pdf = light.PdfLi(isect, wi);
					if (light_pdf > 0) {
						double weight = PowerHeuristic(1, pdf, 1, light_pdf);

						Ld += f * Li * weight / pdf;
					}
				}
			}
		}
		return Ld;
	}

	Color EstimateDirectLightPositionMis(SurfaceIntersection& isect, const Light& light,
		Scene* scene, Sampler& sampler, bool skip_specular) {
		Color Ld{};

		bool is_specular = isect.bsdf()->IsDelta();
		if (is_specular && skip_specular)
			return Ld;

		LightLiSample ls = light.SampleLi(isect, sampler.GetVector2d());
		if (!ls.Li.IsBlack() && ls.pdf > 0) {
			if (!scene->Occluded(isect, ls.position)) {
				Color f = isect.bsdf()->F(isect.wo, ls.wi) * AbsDot(ls.wi, isect.normal);
				if (!f.IsBlack()) {
					if (light.IsDelta()) {
						Ld += f * ls.Li / ls.pdf;
					}
					else {
						double bsdf_pdf = isect.bsdf()->Pdf(isect.wo, ls.wi);
						double weight = PowerHeuristic(1, ls.pdf, 1, bsdf_pdf);

						Ld += f * ls.Li * weight / ls.pdf;
					}
				}

			}
		}
		return Ld;
	}

	Color EstimateDirectLightBothMis(SurfaceIntersection& isect, const Light& light,
		Scene* scene, Sampler& sampler, bool skip_specular) {

		Color Lb = EstimateDirectLightDirectionMis(isect, light, scene, sampler, skip_specular);
		Color Ll = EstimateDirectLightPositionMis(isect, light, scene, sampler, skip_specular);

		return Lb + Ll;
	}

	Color SampleAllLight(
		SurfaceIntersection& isect, Scene* scene,
		Sampler& sampler, bool skip_specular, DirectSampleEnum sample_enum) {
		Color Ld{};

		std::function<decltype(EstimateDirectLightBothMis)> estimate_direct_light;
		switch (sample_enum) {
		case DirectSampleEnum::Bsdf:
			estimate_direct_light = EstimateDirectLightDirection;
			break;
		case DirectSampleEnum::Light:
			estimate_direct_light = EstimateDirectLightPosition;
			break;
		case DirectSampleEnum::BsdfMis:
			estimate_direct_light = EstimateDirectLightDirectionMis;
			break;
		case DirectSampleEnum::LightMis:
			estimate_direct_light = EstimateDirectLightPositionMis;
			break;
		case DirectSampleEnum::BothMis:
			estimate_direct_light = EstimateDirectLightBothMis;
			break;
		default:
			break;
		}

		for (const auto& light : scene->lightList) {
			Ld += estimate_direct_light(isect, *light, scene, sampler, skip_specular);
		}

		return Ld;
	}
}