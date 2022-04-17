#pragma once

#include<bxdf/bsdf.h>

namespace schwi {
	class Fresnel :public BSDF {
	private:
		Color R;
		Color T;
		double etaI;
		double etaT;

	public:
		Fresnel(const Frame& shadingFrame,const Color& R,const Color& T,
			double etaI,double etaT):
			BSDF(shadingFrame),R(R),etaI(etaI),etaT(etaT){}

		Color _f(const Vector3d& wo, const Vector3d& wi)const override {
			return Color();
		}

		double _Pdf(const Vector3d& wo, const Vector3d& wi)const override {
			return 0;
		}

		BSDFSample _Sample_f(const Vector3d& wo, const Vector2d& random)const override {
			BSDFSample sample;

			Normal3d normal(0, 0, 1);
			bool into = Dot(normal, wo)>0;

			Normal3d woNormal = into ? normal : -normal;
			double eta = into ? etaI / etaT : etaT / etaI;

			Vector3d reflectionDirection = Vector3d(-wo.x, -wo.y, wo.z);

			double cosThetaI = Dot(wo, woNormal);
			double cosThetaT2 = 1 - eta * eta * (1 - cosThetaI * cosThetaI);
			if (cosThetaT2 < 0) {
				return sample;
			}
			double cosThetaT = sqrt(cosThetaT2);
			Vector3d refractDirection = (-wo * eta + woNormal * (cosThetaI * eta - cosThetaT)).Normalize();

			double a = etaT - etaI;
			double b = etaT + etaI;
			double R0 = a * a / (b * b);
			double c = 1 - (into ? cosThetaI : cosThetaT);

			double Re = R0 + (1 - R0) * pow(c, 5);
			double Tr = 1-Re;

			if (random[0] < Re) {
				sample.wi = reflectionDirection;
				sample.pdf = Re;
				sample.f = (R * Re) / AbsCosTheta(sample.wi);
			}
			else {
				sample.wi = refractDirection;
				sample.pdf = Tr;
				sample.f = (T * Tr) / AbsCosTheta(sample.wi);
			}

			return sample;
		}

	};
}