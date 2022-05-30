#pragma once

#include<material/material.h>

//TODO: SSS
namespace schwi {
	class PreIntegraterdSkin :public Material {
	private:
		TextureSPtr LUT;

	public:
		PreIntegraterdSkin(const TextureSPtr& lut) :
			LUT(lut) {}

		BsdfUPtr Scattering(const SurfaceIntersection& isect)const override {
			Color Kd = LUT->Evaluate(isect);
			return std::make_unique<Lambertion>(Frame(isect.normal), Kd);
		}

		bool IsDelay()const override { return true; }
	};

	class Stockings :public Material {
	private:
		TextureSPtr out;
		TextureSPtr in;
		double exp;
		mutable RNG rng;

	public:
		Stockings(const TextureSPtr& out, const TextureSPtr& in, double exp = 5) :
			out(out), in(in), exp(exp) {}

		BsdfUPtr Scattering(const SurfaceIntersection& isect)const override {
			double cosTheta = max(.0, Dot(isect.normal, isect.wo));
			double prob = std::pow(cosTheta, exp);
			double random = rng.UniformDouble();
			if (random < prob) {
				Color Kt = in->Evaluate(isect);
				return std::make_unique<Lambertion>(Frame(isect.normal), Kt );
			}
			else {
				Color Kr = out->Evaluate(isect);
				return std::make_unique<Lambertion>(Frame(isect.normal), Kr );
			}
		}
	};
}