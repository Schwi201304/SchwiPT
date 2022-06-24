#pragma once

#include<bxdf/bxdf.h>

namespace schwi {
	class PhongSpecularReflection :public BSDF {
	private:
		Color Ks;
		double exp;

	public:
		PhongSpecularReflection(const Frame& shadingFrame, const Color& Ks, double exp) :
			BSDF(shadingFrame), Ks(Ks), exp(exp) {}

		virtual bool IsDelta() const override { return false; }

	private:
		Color f(const Vector3d& wo, const Vector3d& wi)const override {
			Vector3d wr = Reflect(wo, Normal3d());
			double cosAlpha = Dot(wr, wi);
			Color rho = Ks * (exp + 2.) * Inv2Pi;
			return rho * std::pow(cosAlpha, exp);
		}

		double _Pdf(const Vector3d& wo, const Vector3d& wi)const override {
			Vector3d wr = Reflect(wo, Normal3d(0, 0, 1));
			return PdfHemisphereCosinePhong(wr, wi);
		}

	public:
		Color Sample_f(const Vector3d& world_wo, Vector3d* world_wi, double* pdf, const Vector2d& random)const override {
			Vector3d wo = ToLocal(world_wo);
			Vector3d local_wi = SampleHemisphereCosinePhong(random);
			Vector3d wr = Reflect(wo, Normal3d(0, 0, 1));
			Frame frame{ Normal3d(wr) };
			*world_wi = frame.ToWorld(local_wi);

			if (wo.z < 0)
				world_wi->z *= -1;

			*pdf = _Pdf(wo, local_wi);
			return f(wo, local_wi);
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