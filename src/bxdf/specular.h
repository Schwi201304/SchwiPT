#pragma once

#include<bxdf/bsdf.h>

namespace schwi {
	class Specular :public BSDF {
	private:
		Color R;

	public:
		Specular(const Frame& shadingFrame, const Color& R):
			BSDF(shadingFrame), R(R) {}

		Color _f(const Vector3d& wo, const Vector3d& wi)const override {
			return Color();
		}

		double _Pdf(const Vector3d& wo, const Vector3d& wi)const override {
			return 0;
		}

		BSDFSample _Sample_f(const Vector3d& wo, const Vector2d& random)const override {
			BSDFSample sample;

			sample.wi = Vector3d(-wo.x, -wo.y, wo.z);
			sample.pdf = 1;
			sample.f = R / AbsCosTheta(sample.wi);

			return sample;
		}
	};
}