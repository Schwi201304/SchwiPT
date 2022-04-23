#pragma once

#include<core/schwi.h>
#include<math/vector2.h>
#include<math/vector3.h>
#include<image/color.h>
#include<core/frame.h>
#include<samplers/sampler.h>

namespace schwi {

	struct BSDFSample {
		Color f{};
		Vector3d wi{};
		double pdf{};
	};

	enum BxDFType {
		BSDF_REFLECTION = 1 << 0,
		BSDF_TRANSMISSION = 1 << 1,
		BSDF_DIFFUSE = 1 << 2,
		BSDF_GLOSSY = 1 << 3,
		BSDF_SPECULAR = 1 << 4,
		BSDF_ALL = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR | BSDF_REFLECTION |
		BSDF_TRANSMISSION,
	};

	class BSDF {
	public:
		virtual ~BSDF() = default;
		BSDF(Frame shadingFrame)
			:shadingFrame(shadingFrame) {}

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

	inline Vector3d reflect(const Vector3d& wo, const Normal3d& normal) {
		return -wo + 2 * Dot(wo, normal) * normal;
	}
}