#pragma once

#include<core/schwi.h>
#include<samplers/sampler.h>

namespace schwi {
	class RandomSampler :public Sampler{
	public:
		using Sampler::Sampler;

		std::unique_ptr<Sampler> clone()override {
			return std::make_unique<RandomSampler>(SamplePerPixel);
		}

		double GetDouble()override {
			return rng.UniformDouble();
		}

		Vector2d GetVector2d()override {
			return rng.UniformVector2d();
		}

		CameraSample GetCameraSample(Point2d p)override {
			return { p + rng.UniformVector2d() };
		}
	};
}