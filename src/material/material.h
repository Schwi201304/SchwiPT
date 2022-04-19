#pragma once

#include<core/schwi.h>
#include<core/intersection.h>
#include<bxdf/bsdf.h>
#include<bxdf/lambertion.h>
#include<bxdf/specular.h>
#include<bxdf/fresnel.h>

namespace schwi {
	class Material {
	public:
		virtual ~Material() = default;

		virtual BsdfUPtr Scattering(const Intersection& isect)const = 0;
	};

	class Matte :public Material {
	private:
		Color Kd;

	public:
		Matte(const Color& Kd) :
			Kd(Kd) {}

		BsdfUPtr Scattering(const Intersection& isect)const override {
			return std::make_unique<Lambertion>(Frame(isect.normal), Kd);
		}
	};

	class Mirror :public Material {
	private:
		Color Kr;

	public:
		Mirror(const Color& Kr) :
			Kr(Kr) {}

		BsdfUPtr Scattering(const Intersection& isect)const override {
			return std::make_unique<Specular>(Frame(isect.normal), Kr);
		}
	};

	class Dielectric :public Material {
	private:
		Color Kr;
		Color Kt;
		double eta;

	public:
		Dielectric(const Color& Kr, const Color& Kt, double eta) :
			Kr(Kr), Kt(Kt), eta(eta) {}

		BsdfUPtr Scattering(const Intersection& isect)const override {
			return std::make_unique<Fresnel>(Frame(isect.normal), Kr, Kt, 1, eta);
		}
	};
}