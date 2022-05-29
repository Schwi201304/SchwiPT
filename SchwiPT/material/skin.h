#pragma once

#include<material/material.h>

namespace schwi {
	class PreIntegraterdSkin :public Material {
	private:
		TextureSPtr LUT;

	public:
		PreIntegraterdSkin(const TextureSPtr& lut):
			LUT(lut){}

		BsdfUPtr Scattering(const SurfaceIntersection& isect)const override {
			Color Ks = LUT->Evaluate(isect);
			return std::make_unique<PhongSpecularReflection>(Frame(isect.normal), Ks, 1);
		}
	};
}