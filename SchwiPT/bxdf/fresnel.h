#pragma once

#include<bxdf/bxdf.h>

namespace schwi {
	class Fresnel {
	public:
		virtual ~Fresnel() {}
		virtual double Evaluate(double cosI) const = 0;
	};

	double FrDielectric(
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

		double sin_theta_i = sqrt(max(.0, 1 - cos_theta_i * cos_theta_i));
		double sin_theta_t = eta_i / eta_t * sin_theta_i;

		if (sin_theta_t >= 1)
			return 1;

		double cos_theta_t = sqrt(max(.0, 1. - sin_theta_t * sin_theta_t));


		double r_para = ((eta_t * cos_theta_i) - (eta_i * cos_theta_t)) /
			((eta_t * cos_theta_i) + (eta_i * cos_theta_t));
		double r_perp = ((eta_i * cos_theta_i) - (eta_t * cos_theta_t)) /
			((eta_i * cos_theta_i) + (eta_t * cos_theta_t));
		return (r_para * r_para + r_perp * r_perp) / 2;
	}

	double FrConductor(double cosThetaI, const double& etai,
		const double& etat, const double& k) {
		cosThetaI = Clamp(cosThetaI, -1, 1);
		double eta = etat / etai;
		double etak = k / etai;

		double cosThetaI2 = cosThetaI * cosThetaI;
		double sinThetaI2 = 1. - cosThetaI2;
		double eta2 = eta * eta;
		double etak2 = etak * etak;

		double t0 = eta2 - etak2 - sinThetaI2;
		double a2plusb2 = sqrt(t0 * t0 + 4 * eta2 * etak2);
		double t1 = a2plusb2 + cosThetaI2;
		double a = sqrt(0.5 * (a2plusb2 + t0));
		double t2 = (double)2 * cosThetaI * a;
		double Rs = (t1 - t2) / (t1 + t2);

		double t3 = cosThetaI2 * a2plusb2 + sinThetaI2 * sinThetaI2;
		double t4 = t2 * sinThetaI2;
		double Rp = Rs * (t3 - t4) / (t3 + t4);

		return 0.5 * (Rp + Rs);
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


	class FresnelConductor : public Fresnel {
	public:
		FresnelConductor(const double& etaI, const double& etaT,
			const double& k)
			: etaI(etaI), etaT(etaT), k(k) {}
		double Evaluate(double cosThetaI) const {
			return FrConductor(std::abs(cosThetaI), etaI, etaT, k);
		}

	private:
		double etaI, etaT, k;
	};

	class FresnelDielectric : public Fresnel {
	public:
		FresnelDielectric(double etaI, double etaT) :
			etaI(etaI), etaT(etaT) {}
		double Evaluate(double cosThetaI) const {
			return FrDielectric(cosThetaI, etaI, etaT);
		}

	private:
		double etaI, etaT;
	};

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

			double Re = FrDielectric(CosTheta(wo), etaI, etaT);
			double Tr = 1 - Re;

			if (random[0] < Re) {
				sample.wi = Vector3d(-wo.x, -wo.y, wo.z);
				sample.pdf = Re;
				sample.f = (R * Re) / AbsCosTheta(sample.wi);
				sample.type = BxDFType::REFLECTION | BxDFType::SPECULAR;
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
					sample.type = BxDFType::TRANSMISSION | BxDFType::SPECULAR;
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