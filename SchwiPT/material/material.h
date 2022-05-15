#pragma once

#include<core/schwi.h>
#include<core/intersection.h>
#include<textures/texture.h>
#include<bxdf/bsdf.h>
#include<bxdf/lambertion.h>
#include<bxdf/specular.h>
#include<bxdf/fresnel.h>
#include<bxdf/phong.h>

namespace schwi {
	class Material {
	public:
		virtual ~Material() = default;

		virtual BsdfUPtr Scattering(const Intersection& isect)const = 0;
	};

	class Matte :public Material {
	private:
		TextureSPtr texture;

	public:
		Matte(const TextureSPtr& texture) :
			texture(texture) {}

		BsdfUPtr Scattering(const Intersection& isect)const override {
			Color Kd = texture->Evaluate(isect);
			return std::make_unique<Lambertion>(Frame(isect.normal), Kd);
		}
	};

	class Mirror :public Material {
	private:
		TextureSPtr texture;

	public:
		Mirror(const TextureSPtr& texture) :
			texture(texture) {}

		BsdfUPtr Scattering(const Intersection& isect)const override {
			Color Kr = texture->Evaluate(isect);
			return std::make_unique<SpecularReflection>(Frame(isect.normal), Kr);
		}
	};
	
	class Glass :public Material {
	private:
		TextureSPtr TexKr;
		TextureSPtr TexKt;
		double eta;

	public:
		Glass(const TextureSPtr& TexKr, const TextureSPtr& TexKt, double eta) :
			TexKr(TexKr), TexKt(TexKt), eta(eta) {}

		BsdfUPtr Scattering(const Intersection& isect)const override {
			Color Kr = TexKr->Evaluate(isect);
			Color Kt = TexKt->Evaluate(isect);
			return std::make_unique<FresnelSpecular>(Frame(isect.normal), Kr, Kt, 1, eta);
		}
	};
	
	class Plastic :public Material {
	private:
		TextureSPtr TexKd;
		TextureSPtr TexKs;
		double exp;
		mutable RNG rng;

	public:
		Plastic(const TextureSPtr& TexKd, const TextureSPtr& TexKs, double shininess) :
			TexKd(TexKd), TexKs(TexKs), exp(shininess) {
		}

		BsdfUPtr Scattering(const Intersection& isect)const override {
			Color Kd = TexKd->Evaluate(isect);
			Color Ks = TexKd->Evaluate(isect);

			double diffuse = Kd.Luminance();
			double specular = Ks.Luminance();
			double luminance = diffuse + specular;

			double diffuseProbility = diffuse / luminance;
			double specularProbility = specular / luminance;

			double random = rng.UniformDouble();
			if (random < specularProbility) {
				return std::make_unique<PhongSpecularReflection>(Frame(isect.normal), Ks / specularProbility, exp);
			}
			else {
				return std::make_unique<Lambertion>(Frame(isect.normal), Kd / diffuseProbility);
			}
		}
	};
}