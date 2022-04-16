#pragma once

#include<core/schwi.h>
#include<math/RNG.h>
#include<math/point2.h>

namespace schwi {

	struct CameraSample
	{
		Point2d SamplePoint;
	};

	class Sampler {
	protected:
		RNG rng;
		int SamplePerPixel;
		int CurrentSampleIndex;

	public:
		Sampler(int spp) :SamplePerPixel(spp) {}

		virtual int GetSPP() {
			return SamplePerPixel;
		}

		virtual void SetSPP(int spp) {
			SamplePerPixel = spp;
		}

		virtual std::unique_ptr<Sampler> clone() = 0;

		virtual void StartPixel() {
			CurrentSampleIndex = 0;
		}

		virtual bool NextSample(){
			CurrentSampleIndex += 1;
			return CurrentSampleIndex < SamplePerPixel;
		}

	public:
		virtual double GetDouble() = 0;
		virtual Vector2d GetVector2d() = 0;
		virtual CameraSample GetCameraSample(Point2d p) = 0;
	};
}