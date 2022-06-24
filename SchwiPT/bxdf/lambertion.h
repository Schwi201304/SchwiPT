#pragma once

#include<bxdf/bxdf.h>

namespace schwi {
	class Lambertion :public BSDF {
	private:
		Color R;

	public:
		Lambertion(const Frame& shadingFrame, const Color& R) :
			BSDF(shadingFrame), R(R) {}

		virtual bool IsDelta()const override { return false; }

		Color Sample_f(const Vector3d& world_wo, Vector3d* world_wi, double* pdf, const Vector2d& random)const override {
			Vector3d wo = ToLocal(world_wo);
			Vector3d local_wi = SampleHemisphereCosine(random);
			if (wo.z < 0)
				local_wi.z *= -1;

			*world_wi = ToWorld(local_wi);
			*pdf = _Pdf(wo, local_wi);
			return f(wo, local_wi);
		}

	private:
		Color f(const Vector3d& wo, const Vector3d& wi)const override {
			return R * InvPi;
		}

		double _Pdf(const Vector3d& wo, const Vector3d& wi)const override {
			return SameHemisphere(wo, wi) ?
				PdfHemisphereCosine(AbsCosTheta(wi)) :
				0;
		}
	};
}