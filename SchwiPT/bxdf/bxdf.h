#pragma once

#include<core/schwi.h>
#include<core/color.h>
#include<core/frame.h>
#include<samplers/sampler.h>
#include<samplers/sampling.h>

namespace schwi {

	enum class BxDFType {
		NONE = 0,
		REFLECTION = 1 << 0,
		TRANSMISSION = 1 << 1,
		SCATTERING = REFLECTION | TRANSMISSION,
		DIFFUSE = 1 << 2,
		GLOSSY = 1 << 3,
		SPECULAR = 1 << 4,
		ALL = REFLECTION | TRANSMISSION | DIFFUSE | GLOSSY | SPECULAR
	};

	BxDFType operator|(BxDFType a, BxDFType b) {
		return BxDFType((int)a | (int)b);
	}

	BxDFType operator&(BxDFType a, BxDFType b) {
		return BxDFType((int)a & (int)b);
	}

	inline bool IsDeltaBSDF(BxDFType type) {
		return ((int)type & (int)BxDFType::SPECULAR) > 0;
	}

	class BxDF {
	public:
		const BxDFType type;

		virtual ~BxDF() {}
		BxDF(BxDFType type) :type(type) {}

		bool MatchFlag(BxDFType t) const {
			return (type & t) == type;
		}

		virtual Color f(const Vector3d& wo, const Vector3d& wi)const = 0;
		virtual Color Sample_f(const Vector3d& w, const Point2d& sample) const;
		virtual Color rho(const Vector3d& wo, int nSamples,
			const Point2d* samples) const;
		virtual Color rho(int nSamples, const Point2d* samples1,
			const Point2d* samples2) const;
		virtual double Pdf(const Vector3d& wo, const Vector3d& wi) const;
	};

	class BSDF {
	public:
		virtual ~BSDF() = default;
		BSDF(Frame shadingFrame)
			:shadingFrame(shadingFrame) {}

		virtual bool IsDelta()const = 0;

		Color F(const Vector3d& wo, const Vector3d& wi)const {
			return f(ToLocal(wo), ToLocal(wi));
		}

		double Pdf(const Vector3d& wo, const Vector3d& wi)const {
			return _Pdf(ToLocal(wo), ToLocal(wi));
		}

		virtual Color Sample_f(const Vector3d& world_wo, Vector3d* world_wi, double* pdf, const Vector2d& random)const = 0;

	protected:
		virtual Color f(const Vector3d& wo, const Vector3d& wi)const = 0;
		virtual double _Pdf(const Vector3d& wo, const Vector3d& wi)const = 0;

	protected:
		Vector3d ToLocal(const Vector3d& world)const {
			return shadingFrame.ToLocal(world);
		}

		Vector3d ToWorld(const Vector3d& local)const {
			return shadingFrame.ToWorld(local);
		}

	private:
		Frame shadingFrame;
		int nBxDFs = 0;
		static constexpr int MaxBxDFs = 4;
		BxDF* bxdfs[MaxBxDFs];
	};

	inline double CosTheta(const Vector3d& w) {
		return w.z;
	}

	inline double Cos2Theta(const Vector3d& w) {
		return w.z * w.z;
	}

	inline double AbsCosTheta(const Vector3d& w) {
		return std::abs(w.z);
	}

	inline double Sin2Theta(const Vector3d& w) {
		return std::max(0., 1. - Cos2Theta(w));
	}

	inline double SinTheta(const Vector3d& w) {
		return std::sqrt(Sin2Theta(w));
	}

	inline double TanTheta(const Vector3d& w) {
		return SinTheta(w) / CosTheta(w);
	}

	inline double Tan2Theta(const Vector3d& w) {
		return Sin2Theta(w) / Cos2Theta(w);
	}

	inline double CosPhi(const Vector3d& w) {
		double sinTheta = SinTheta(w);
		return (sinTheta == 0) ? 1 : Clamp(w.x / sinTheta, -1, 1);
	}

	inline double SinPhi(const Vector3d& w) {
		double sinTheta = SinTheta(w);
		return (sinTheta == 0) ? 0 : Clamp(w.y / sinTheta, -1, 1);
	}

	inline double Cos2Phi(const Vector3d& w) {
		return CosPhi(w) * CosPhi(w);
	}

	inline double Sin2Phi(const Vector3d& w) {
		return SinPhi(w) * SinPhi(w);
	}

	inline bool SameHemisphere(const Vector3d& w, const Vector3d& wp) {
		return w.z * wp.z > 0;
	}

	inline bool SameHemisphere(const Vector3d& w, const Normal3d& wp) {
		return w.z * wp.z > 0;
	}

	inline Vector3d Reflect(const Vector3d& wo, const Normal3d& normal) {
		return -wo + 2 * Dot(wo, normal) * normal;
	}

	inline bool Refract(const Vector3d& wi, const Normal3d& normal, double eta, Vector3d* wt)
	{
		double cosThetaI = Dot(normal, wi);
		double sin2ThetaI = std::max(0., 1. - cosThetaI * cosThetaI);
		double sin2ThetaT = eta * eta * sin2ThetaI;

		if (sin2ThetaT >= 1)
			return false;

		double cosThetaT = std::sqrt(1 - sin2ThetaT);
		*wt = eta * -wi + (eta * cosThetaI - cosThetaT) * Vector3d(normal);

		return true;
	}
}