#pragma once

#include<core/schwi.h>
#include<core/ray.h>
#include<math/vector2.h>
#include<image/color.h>
#include<samplers/sampler.h>
#include<cameras/camera.h>

namespace schwi {
	class Integrator {
	public:
		~Integrator() = default;
		Integrator() {}

		void Render(Scene& scene, Camera& camera, Sampler& originalSampler, SchwiImage& img) {
			auto [width, height] = img.resolution();

#ifndef _DEBUG
#pragma omp parallel for schedule(dynamic,1)
#endif
			printf("SPP:%d,Resolution:%d*%d\n", originalSampler.GetSPP(), width, height);
			for (int y = 0; y < height; y++) {
				std::unique_ptr<Sampler> sampler = originalSampler.Clone();
				std::cerr << "\rRendered:" << y + 1 << std::flush;
				for (int x = 0; x < width; x++) {
					Color L{};
					sampler->StartPixel();
					do {
						auto cameraSample = sampler->GetCameraSample(Point2d(x, y));
						auto ray = camera.GenerateRay(cameraSample);
						
						L = L + Li(ray, scene, *sampler) / sampler->GetSPP();
					} while (sampler->NextSample());
					img.setColor(x, y, ToByte(L));
				}
			}
		}

		virtual Color Li(Ray ray, Scene& scene, Sampler& sampler) = 0;
	};
}