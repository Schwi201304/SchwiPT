#pragma once

#include<core/schwi.h>
#include<shapes/shape.h>
#include<material/material.h>
#include<lights/light.h>
#include<textures/texture.h>

namespace schwi {
	struct Primitive {
		const Shape* shape;
		const Material* material;
		const Texture<Color>* texture;
		const AreaLight* areaLight;

		bool Intersect(Ray& ray, Intersection* isect)const {
			bool hit = shape->Intersect(ray, isect);
			if (hit) {
				isect->bsdfPtr = material->Scattering(*isect);
				isect->emission = areaLight ?
					areaLight->Le(*isect, isect->wo) :
					texture ?
					texture->Evaluate(*isect) :
					Color();
			}

			return hit;
		}
	};

}