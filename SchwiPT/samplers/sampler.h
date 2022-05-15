#pragma once

#include<core/schwi.h>

namespace schwi {

	class Sampler {
	protected:
		RNG rng{};
		int SamplePerPixel{};
		int CurrentSampleIndex{};

	public:
		virtual ~Sampler() {}
		Sampler(int spp) :SamplePerPixel(spp) {}

		virtual int GetSPP() {
			return SamplePerPixel;
		}

		virtual void SetSPP(int spp) {
			SamplePerPixel = spp;
		}

		virtual std::unique_ptr<Sampler> Clone() = 0;

		virtual void StartPixel() {
			CurrentSampleIndex = 0;
		}

		virtual bool NextSample() {
			CurrentSampleIndex += 1;
			return CurrentSampleIndex < SamplePerPixel;
		}

	public:
		virtual double GetDouble() = 0;
		virtual Vector2d GetVector2d() = 0;
		virtual CameraSample GetCameraSample(Point2d p) = 0;
	};
}