#pragma once

#include<bxdf/bsdf.h>

namespace schwi {
	double FresnelDielectric(
		double cos_theta_i,
		double eta_i, double eta_t)
	{
		cos_theta_i = Clamp(cos_theta_i, -1, 1);

		bool entering = cos_theta_i > 0.;
		if (!entering)
		{
			std::swap(eta_i, eta_t);
			cos_theta_i = abs(cos_theta_i);
		}

		double sin_theta_i = sqrt(max((double)0, 1 - cos_theta_i * cos_theta_i));
		double sin_theta_t = eta_i / eta_t * sin_theta_i;

		if (sin_theta_t >= 1)
			return 1;

		double cos_theta_t = sqrt(max(0., 1. - sin_theta_t * sin_theta_t));


		double r_para = ((eta_t * cos_theta_i) - (eta_i * cos_theta_t)) /
			((eta_t * cos_theta_i) + (eta_i * cos_theta_t));
		double r_perp = ((eta_i * cos_theta_i) - (eta_t * cos_theta_t)) /
			((eta_i * cos_theta_i) + (eta_t * cos_theta_t));
		return (r_para * r_para + r_perp * r_perp) / 2;
	}

	double FresnelDielectricSchlick(
		double cos_theta_i, double cos_theta_t,
		double eta_i, double eta_t)
	{

		double R0 = (eta_t - eta_i) / (eta_t + eta_i);
		R0 *= R0;

		//double cos_i = eta_i < eta_t ? cos_theta_i : cos_theta_t;
		double cos_i = cos_theta_i < 0 ? -cos_theta_i : cos_theta_t;

		return Lerp(R0, 1., pow(1 - cos_i, 5.));
	}

	double FresnelDielectricSchlick(
		double cos_theta_i,
		double eta_i, double eta_t)
	{
		double R0 = (eta_t - eta_i) / (eta_t + eta_i);
		R0 *= R0;

		return Lerp(R0, 1., pow(1 - cos_theta_i, 5.));
	}

	double FresnelDielectricSchlick(double cos_theta_i, double R0)
	{
		return Lerp(R0, 1., pow(1 - cos_theta_i, 5.));
	}

	//TODO ¸ÄÐ´·ÆÄù¶û
	class FresnelSpecular :public BSDF {
	private:
		Color R;
		Color T;
		double etaI;
		double etaT;

	public:
		FresnelSpecular(const Frame& shadingFrame, const Color& R, const Color& T,
			double etaI, double etaT) :
			BSDF(shadingFrame), R(R), T(T), etaI(etaI), etaT(etaT) {}

		bool IsDelta()const override { return true; }

		Color _f(const Vector3d& wo, const Vector3d& wi)const override {
			return Color();
		}

		double _Pdf(const Vector3d& wo, const Vector3d& wi)const override {
			return 0;
		}

		BSDFSample _Sample_f(const Vector3d& wo, const Vector2d& random)const override {
			BSDFSample sample;

			double Re = FresnelDielectric(CosTheta(wo), etaI, etaT);
			double Tr = 1 - Re;

			if (random[0] < Re) {
				sample.wi = Vector3d(-wo.x, -wo.y, wo.z);
				sample.pdf = Re;
				sample.f = (R * Re) / AbsCosTheta(sample.wi);
				sample.type = BSDFEnum::REFLECTION | BSDFEnum::SPECULAR;
			}
			else {
				Normal3d normal(0, 0, 1);
				bool into = Dot(normal, wo) > 0;

				Normal3d wo_normal = into ? normal : normal * -1;
				double eta = into ? etaI / etaT : etaT / etaI;

				if (Refract(wo, wo_normal, eta, &sample.wi))
				{
					sample.pdf = Tr;
					sample.f = (T * Tr) / AbsCosTheta(sample.wi);
					sample.type = BSDFEnum::TRANSMISSION | BSDFEnum::SPECULAR;
				}
				else
				{
					sample.f = Color(); // total internal reflection
				}
			}

			return sample;
		}

	public:
		static constexpr double Vacuum = 1.0;
		static constexpr double Air = 1.00029;//At sea level
		static constexpr double Ice = 1.31;
		static constexpr double Water = 1.333;//20¡æ
		static constexpr double Glass = 1.5;
		static constexpr double Diamond = 2.42;
	};
}