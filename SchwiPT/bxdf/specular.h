#pragma once

#include<bxdf/bxdf.h>
#include<bxdf/fresnel.h>

namespace schwi {
	class SpecularReflection :public BSDF {
	private:
		Color R;
		const Fresnel* fresnel;

	public:
		SpecularReflection(const Frame& shadingFrame, const Color& R, Fresnel* fresnel) :
			BSDF(shadingFrame), R(R), fresnel(fresnel) {}

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
			return fresnel->Evaluate(CosTheta(local_wi)) * R / AbsCosTheta(local_wi);
		}
	};


	class SpecularTransmission :public BSDF {
	private:
		Color T;
		double etaA, etaB;
		FresnelDielectric fresnel;

	public:
		SpecularTransmission(const Frame& shadingFrame, const Color& T,
			double etaA, double etaB) :
			BSDF(shadingFrame), T(T),
			etaA(etaA), etaB(etaB),
			fresnel(etaA, etaB) {}

		virtual bool IsDelta()const override { return true; }

		Color f(const Vector3d& wo, const Vector3d& wi)const override {
			return Color();
		}

		double _Pdf(const Vector3d& wo, const Vector3d& wi)const override {
			return 0;
		}

		Color Sample_f(const Vector3d& world_wo, Vector3d* world_wi, double* pdf, const Vector2d& random)const override {
			Vector3d local_wi;
			Vector3d local_wo = ToLocal(world_wo);
			bool entering = CosTheta(local_wo) > 0;
			double etaI = entering ? etaA : etaB;
			double etaT = entering ? etaB : etaA;

			Normal3d normal(0, 0, 1);
			bool into = Dot(normal, local_wo) > 0;
			Normal3d wo_normal = entering ? normal : normal * -1;
			if (!Refract(local_wo, wo_normal, etaI / etaT, &local_wi))
				return Color();

			*pdf = 1;
			Color ft = T * (1- fresnel.Evaluate(CosTheta(local_wi)));
			*world_wi = ToWorld(local_wi);
			return ft / AbsCosTheta(local_wi);
		}
	};
}