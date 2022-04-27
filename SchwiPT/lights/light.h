#pragma once

#include<core/schwi.h>
#include<core/intersection.h>
#include<core/color.h>

namespace schwi{
	class Light{};

	class AreaLight :public Light {
	private:
		Color radiance;
		const Shape* shape;

	public:
		AreaLight(const Color& radiance,const Shape* shape):
			radiance(radiance),shape(shape){}

		Color Le(const Intersection& lightIsect, const Vector3d& wo)const {
			return (Dot(lightIsect.normal, wo) > 0) ?
				radiance :
				Color(wo.x,wo.y,wo.z);
		}
	};
}