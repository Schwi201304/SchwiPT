#pragma once

#include<samplers/sampler.h>
#include<cameras/camera.h>

namespace schwi {
	class RandomSampler :public Sampler {
	public:
		using Sampler::Sampler;

		std::unique_ptr<Sampler> Clone()override {
			return std::make_unique<RandomSampler>(SamplePerPixel);
		}

		double GetDouble()override {
			return rng.UniformDouble();
		}

		Vector2d GetVector2d()override {
			return rng.UniformVector2d();
		}

		CameraSample GetCameraSample(Point2d p)override {
			return { p + GetVector2d() };
		}
	};

	class CRandomSampler :public Sampler {
	public:
		using Sampler::Sampler;

		std::unique_ptr<Sampler> Clone()override {
			return std::make_unique<CRandomSampler>(SamplePerPixel);
		}

		double GetDouble()override {
			return std::rand() / (RAND_MAX + 1.);
		}

		Vector2d GetVector2d()override {
			return Vector2d(GetDouble(), GetDouble());
		}

		CameraSample GetCameraSample(Point2d p)override {
			return { p + GetVector2d() };
		}
	};
}