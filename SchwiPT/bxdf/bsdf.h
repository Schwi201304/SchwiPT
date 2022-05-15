#pragma once

#include<core/schwi.h>
#include<core/color.h>
#include<core/frame.h>
#include<samplers/sampler.h>
#include<samplers/sampling.h>

namespace schwi {

	enum class BSDFEnum {
		NONE = 0,
		REFLECTION = 1 << 0,
		TRANSMISSION = 1 << 1,
		SCATTERING = REFLECTION | TRANSMISSION,
		DIFFUSE = 1 << 2,
		GLOSSY = 1 << 3,
		SPECULAR = 1 << 4,
		ALL = REFLECTION | TRANSMISSION | DIFFUSE | GLOSSY | SPECULAR
	};

	BSDFEnum operator|(BSDFEnum a, BSDFEnum b) {
		return BSDFEnum((int)a | (int)b);
	}

	BSDFEnum operator&(BSDFEnum a, BSDFEnum b) {
		return BSDFEnum((int)a & (int)b);
	}

	inline bool IsDeltaBSDF(BSDFEnum type) {
		return ((int)type & (int)BSDFEnum::SPECULAR) > 0;
	}

	struct BSDFSample {
		Color f{};
		Vector3d wi{};
		double pdf{};
		BSDFEnum type;
	};

	class BSDF {
	public:
		virtual ~BSDF() = default;
		BSDF(Frame shadingFrame)
			:shadingFrame(shadingFrame) {}

		virtual bool IsDelta()const = 0;

		Color f(const Vector3d& world_wo, const Vector3d& world_wi)const {
			return _f(ToLocal(world_wo), ToLocal(world_wi));
		}

		double Pdf(const Vector3d& world_wo, const Vector3d& world_wi)const {
			return _Pdf(ToLocal(world_wo), ToLocal(world_wi));
		}

		BSDFSample Sample_f(const Vector3d& world_wo, const Vector2d random)const {
			auto sample = _Sample_f(ToLocal(world_wo), random);
			sample.wi = ToWorld(sample.wi);
			return sample;
		}

	protected:
		virtual Color _f(const Vector3d& wo, const Vector3d& wi)const = 0;
		virtual double _Pdf(const Vector3d& wo, const Vector3d& wi)const = 0;
		virtual BSDFSample _Sample_f(const Vector3d& wo, const Vector2d& random)const = 0;

	private:
		Vector3d ToLocal(const Vector3d& world)const {
			return shadingFrame.ToLocal(world);
		}

		Vector3d ToWorld(const Vector3d& local)const {
			return shadingFrame.ToWorld(local);
		}

	private:
		Frame shadingFrame;
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

	inline bool Refract(const Vector3d& wo, const Normal3d& wo_normal, double eta, Vector3d* out_wt)
	{
		double cos_theta_i = Dot(wo_normal, wo);
		double sin_theta_i_sq = std::max(0., 1. - cos_theta_i * cos_theta_i);
		double sin_theta_t_sq = eta * eta * sin_theta_i_sq;

		if (sin_theta_t_sq >= 1)
			return false;

		double cos_theta_t = std::sqrt(1 - sin_theta_t_sq);
		*out_wt = eta * -wo + (eta * cos_theta_i - cos_theta_t) * Vector3d(wo_normal);

		return true;
	}
}