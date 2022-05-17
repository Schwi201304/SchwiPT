#pragma once

#include<core/schwi.h>
#include<core/color.h>
#include<core/ray.h>
#include<bxdf/medium.h>

namespace schwi {
	class Intersection {
	public:
		Point3d position{};
		Normal3d normal{};
		Vector3d wo{};
		MediumInterface mediumInterface;

		Intersection() = default;
		Intersection(Point3d p, Normal3d n, Vector3d wo) :
			position(p), normal(n), wo(wo) {}
		Intersection(Point3d p, Vector3d wo) :
			position(p), wo(wo) {}

		bool IsSurfaceInteraction() const { return normal != Normal3d(); }
		bool IsMediumInteraction() const { return !IsSurfaceInteraction(); }

		Ray GenerateRay(const Vector3d& direction)const {
			return Ray(position, direction);
		}

		Ray GenerateRay(const Point3d& target)const {
			return Ray(position, (target - position).Normalize());
		}

		Ray GenerateRay(const Intersection& isect)const {
			return Ray(position, (isect.position - position).Normalize());
		}

		const Medium* GetMedium(const Vector3d& w) const {
			return Dot(w, normal) > 0 ? mediumInterface.outside : mediumInterface.inside;
		}

		const Medium* GetMedium() const {
			return mediumInterface.inside;
		}
	};

	class MediumIntersection :public Intersection {
	public:
		const PhaseFunction* phase;

		MediumIntersection() : phase(nullptr) {}
		MediumIntersection(const Point3d& p, const Vector3d& wo,
			const Medium* medium, const PhaseFunction* phase)
			: Intersection(p, wo), phase(phase) {}
		bool IsValid() const { return phase != nullptr; }
	};

	class SurfaceIntersection :public Intersection {
	public:
		Point2d uv{};

	private:
		Color emission{};
		BsdfUPtr bsdfPtr{};
		const Shape* shape;
		const Primitive* primitive;
		friend Primitive;

	public:
		SurfaceIntersection() = default;
		SurfaceIntersection(Point3d p, Normal3d n, Vector3d wo, Point2d uv, const Shape* shape)
			:Intersection(p, n, wo), uv(uv), shape(shape) {}

		const Shape* GetShape()const { return shape; }
		const BSDF* bsdf() const { return bsdfPtr.get(); }
		Color Le()const { return emission; }


	};
}