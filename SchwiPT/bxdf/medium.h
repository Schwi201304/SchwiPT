#pragma once

#include<core/schwi.h>

namespace schwi {
	class PhaseFunction {
	public:
		virtual ~PhaseFunction() {}
		virtual double p(const Vector3d& wo, const Vector3d& wi)const = 0;

	};

	inline double PhaseHG(double cosTheta, double g) {
		double denom = 1 + g * g + 2 * g * cosTheta;
		return Inv4Pi * (1 - g * g) / (denom * sqrt(denom));
	}

	class Medium {
	public:
		virtual ~Medium() {}
		virtual Color Tr(const Ray& ray, Sampler& sampler) const = 0;
	};


	class HenyeyGreenstein : public PhaseFunction {
	public:
		HenyeyGreenstein(double g) : g(g) {}
		double p(const Vector3d& wo, const Vector3d& wi) const {
			return PhaseHG(Dot(wo, wi), g);
		}

		double Sample_p(const Vector3d& wo, Vector3d* wi, const Point2d& sample) const;

	private:
		const double g;
	};

	struct MediumInterface {
		const Medium* inside, * outside;
		MediumInterface() : inside(nullptr), outside(nullptr) {}
		MediumInterface(const Medium* medium) : inside(medium), outside(medium) {}
		MediumInterface(const Medium* inside, const Medium* outside)
			: inside(inside), outside(outside) {}
		bool IsMediumTransition() const { return inside != outside; }
	};
}