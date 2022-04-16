#pragma once

#include<core/schwi.h>
#include<math/vector2.h>
#include<math/vector3.h>
#include<image/color.h>
#include<core/frame.h>

namespace schwi {

	struct BSDFSample {
		Color f;
		Vector3d wi;
		double pdf;
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
}