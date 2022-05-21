#pragma once

#include<bxdf/bxdf.h>

namespace schwi {
	class Lambertion :public BSDF {
	private:
		Color R;

	public:
		Lambertion(const Frame& shadingFrame, const Color& R):
			BSDF(shadingFrame), R(R) {}

		virtual bool IsDelta()const override { return false; }

		Color _f(const Vector3d& wo, const Vector3d& wi)const override {
			return R * InvPi;
		}

		double _Pdf(const Vector3d& wo, const Vector3d& wi)const override {
			return SameHemisphere(wo, wi) ? 
				PdfHemisphereCosine(AbsCosTheta(wi)) : 
				0;
		}

		BSDFSample _Sample_f(const Vector3d& wo, const Vector2d& random)const override {
			BSDFSample sample;

			sample.wi = SampleHemisphereCosine(random);
			if (wo.z < 0)
				sample.wi.z *= -1;

			sample.pdf = _Pdf(wo, sample.wi);
			sample.f = _f(wo, sample.wi);
			sample.type = BxDFType::REFLECTION | BxDFType::DIFFUSE;

			return sample;
		}

	};
}