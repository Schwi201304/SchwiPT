#pragma once

#include<core/schwi.h>
#include<bxdf/bsdf.h>

namespace schwi {
	class Phong :public BSDF {
	private:
		Color Ks;
		double exp;

	public:
		Phong(const Frame& shadingFrame, const Color& Ks, double exp) :
			BSDF(shadingFrame), Ks(Ks), exp(exp) {}

		Color _f(const Vector3d& wo, const Vector3d& wi)const override {
			Vector3d wr = reflect(wo, Normal3d());
			double cosAlpha = Dot(wr, wi);
			Color rho = Ks * (exp + 2.) * Inv2Pi;
			return rho * std::pow(cosAlpha, exp);
		}

		double _Pdf(const Vector3d& wo, const Vector3d& wi)const override {
			Vector3d wr = reflect(wo, Normal3d(0, 0, 1));
			return PdfHemisphereCosinePhong(wr, wi);
		}

		BSDFSample _Sample_f(const Vector3d& wo, const Vector2d& random)const override {
			BSDFSample sample;

			sample.wi = SampleHemisphereCosinePhong(random);

			Vector3d wr = reflect(wo, Normal3d(0, 0, 1));
			Frame frame{ Normal3d(wr) };
			sample.wi = frame.ToWorld(sample.wi);

			if (wo.z < 0)
				sample.wi.z *= 1;

			sample.f = _f(wo, sample.wi);
			sample.pdf = _Pdf(wo, sample.wi);
			
			return sample;
		}

	private:
		double PdfHemisphereCosinePhong(const Vector3d& normal, const Vector3d& direction)const {
			double cosTheta = std::max(.0, Dot(normal, direction));
			return (exp + 1.) * std::pow(cosTheta, exp) * Inv2Pi;
		}

		Vector3d SampleHemisphereCosinePhong(const Vector2d& random)const {
			double phi = 2. * Pi * random[0];
			double cosTheta = std::pow(random[1], 1. / (exp + 1.));
			double sinTheta = std::sqrt(1. - cosTheta * cosTheta);
			return Vector3d(std::cos(phi) * sinTheta, std::sin(phi) * sinTheta, cosTheta);
		}
	};
}