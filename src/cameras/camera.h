#pragma once

#include<core/schwi.h>
#include<math/vector3.h>
#include<math/point2.h>

namespace schwi {
	class Camera {
	public:
		Camera(){}

		virtual Ray GenerateRay(const CameraSample& sample)const = 0;
	};

	struct CameraSample
	{
		Point2d SamplePoint;
	};
}