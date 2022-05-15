#pragma once

#include<bxdf/bsdf.h>

namespace schwi {
	class PhongSpecularReflection :public BSDF {
	private:
		Color Ks;
		double exp;

	public:
		PhongSpecularReflection(const Frame& shadingFrame, const Color& Ks, double exp) :
			BSDF(shadingFrame), Ks(Ks), exp(exp) {}

		virtual bool IsDelta() const override { return false; }

		Color _f(const Vector3d& wo, const Vector3d& wi)const override {
			Vector3d wr = Reflect(wo, Normal3d());
			double cosAlpha = Dot(wr, wi);
			Color rho = Ks * (exp + 2.) * Inv2Pi;
			return rho * std::pow(cosAlpha, exp);
		}

		double _Pdf(const Vector3d& wo, const Vector3d& wi)const override {
			Vector3d wr = Reflect(wo, Normal3d(0, 0, 1));
			return PdfHemisphereCosinePhong(wr, wi);
		}

		BSDFSample _Sample_f(const Vector3d& wo, const Vector2d& random)const override {
			BSDFSample sample;

			sample.wi = SampleHemisphereCosinePhong(random);

			Vector3d wr = Reflect(wo, Normal3d(0, 0, 1));
			Frame frame{ Normal3d(wr) };
			sample.wi = frame.ToWorld(sample.wi);

			if (wo.z < 0)
				sample.wi.z *= 1;

			sample.f = _f(wo, sample.wi);
			sample.pdf = _Pdf(wo, sample.wi);
			sample.type = BSDFEnum::REFLECTION | BSDFEnum::GLOSSY;

			return sample;
		}

	private:
		double PdfHemisphereCosinePhong(const Vector3d& normal, const Vector3d& direction)const {
			double cosTheta = max(.0, Dot(normal, direction));
			return (exp + 1.) * pow(cosTheta, exp) * Inv2Pi;
		}

		Vector3d SampleHemisphereCosinePhong(const Vector2d& random)const {
			double phi = 2. * Pi * random[0];
			double cosTheta = pow(random[1], 1. / (exp + 1.));
			double sinTheta = sqrt(1. - cosTheta * cosTheta);
			return Vector3d(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
		}
	};
}