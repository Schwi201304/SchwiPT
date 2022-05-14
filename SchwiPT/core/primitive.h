#pragma once

#include<core/schwi.h>
#include<shapes/shape.h>
#include<material/material.h>
#include<lights/light.h>
#include<textures/texture.h>

namespace schwi {
	struct Primitive {
		const Shape* shape;
		const Material* out;
		const AreaLight* areaLight;
		//const Material* in=nullptr;

		bool Intersect(Ray& ray, Intersection* isect)const {
			bool hit = shape->Intersect(ray, isect);
			if (hit) {
				isect->bsdfPtr = out->Scattering(*isect);
				//isect->bsdfPtr = Dot(isect->wo,isect->normal)>0?
				//	out->Scattering(*isect):
				//	in?
				//	in->Scattering(*isect):
				//	out->Scattering(*isect);
				isect->emission = areaLight ?
					areaLight->Le(*isect, isect->wo) :
					Color();
			}

			return hit;
		}
	};

}