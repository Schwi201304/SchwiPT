#pragma once

#include<core/schwi.h>
#include<math/vector3.h>
#include<samplers/sampler.h>

namespace schwi {
	class Camera {
	public:
		Camera(){}

		virtual Ray GenerateRay(const CameraSample& sample)const = 0;
	};
}