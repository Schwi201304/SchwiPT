#pragma once

#include<bxdf/bxdf.h>

namespace schwi {
	class SpecularReflection :public BSDF {
	private:
		Color R;
		const Fresnel* fresnel;

	public:
		SpecularReflection(const Frame& shadingFrame, const Color& R) :
			BSDF(shadingFrame), R(R) {}

		virtual bool IsDelta() const override { return true; }

		Color f(const Vector3d& wo, const Vector3d& wi)const override {
			return Color();
		}

		double _Pdf(const Vector3d& wo, const Vector3d& wi)const override {
			return 0;
		}

		Color Sample_f(const Vector3d& world_wo, Vector3d* world_wi, double* pdf, const Vector2d& random)const override {
			Vector3d wo = ToLocal(world_wo);
			Vector3d local_wi(-wo.x, -wo.y, wo.z);
			*world_wi = ToWorld(local_wi);
			*pdf = 1;
			return R / AbsCosTheta(local_wi);
		}
	};

	/*
	class SpecularTransmission :public BSDF {
	private:
		Color T;

	public:
		SpecularTransmission(const Frame& shadingFrame, const Color& T) :
			BSDF(shadingFrame), T(T) {}

		virtual bool IsDelta()const override { return true; }

		Color f(const Vector3d& wo, const Vector3d& wi)const override {
			return Color();
		}

		double Pdf(const Vector3d& wo, const Vector3d& wi)const override {
			return 0;
		}

		Color Sample_f(const Vector3d& wo, Vector3d* world_wi, double* pdf, const Vector2d& random)const override {

			Vector3d local_wi(-wo.x, -wo.y, wo.z);
			*world_wi = ToWorld(local_wi);
			*pdf = 1;
			return T / AbsCosTheta(local_wi);
		}
	};*/
}