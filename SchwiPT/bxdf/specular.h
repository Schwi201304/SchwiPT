#pragma once

#include<bxdf/bsdf.h>

namespace schwi {
	class SpecularReflection :public BSDF {
	private:
		Color R;

	public:
		SpecularReflection(const Frame& shadingFrame, const Color& R):
			BSDF(shadingFrame), R(R) {}

		virtual bool IsDelta() const override { return true; }

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
			sample.type = BSDFEnum::REFLECTION | BSDFEnum::SPECULAR;

			return sample;
		}
	};

	class SpecularTransmission :public BSDF {
	private:
		Color T;

	public:
		SpecularTransmission(const Frame& shadingFrame,const Color& T):
			BSDF(shadingFrame),T(T){}

		virtual bool IsDelta()const override { return true; }

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
			sample.f = T / AbsCosTheta(sample.wi);
			sample.type = BSDFEnum::TRANSMISSION | BSDFEnum::SPECULAR;

			return sample;
		}
	};
}