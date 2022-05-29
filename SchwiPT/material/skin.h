#pragma once

#include<material/material.h>

//TODO: SSS
namespace schwi {
	class PreIntegraterdSkin :public Material {
	private:
		TextureSPtr LUT;

	public:
		PreIntegraterdSkin(const TextureSPtr& lut):
			LUT(lut){}

		BsdfUPtr Scattering(const SurfaceIntersection& isect)const override {
			Color Kd = LUT->Evaluate(isect);
			return std::make_unique<Lambertion>(Frame(isect.normal), Kd);
		}

		bool IsDelay()const override{ return true; }
	};
}