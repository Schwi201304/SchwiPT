#pragma once

#include<core/schwi.h>

namespace schwi {
	class Filter {
	public:
		const Vector2d radius;
		const Vector2d invRadius;

		virtual ~Filter(){}
		Filter(const Vector2d& radius)
			:radius(radius),invRadius{1/radius.x,1/radius.y}{}

		virtual double Evaluate(const Point2d& p)const = 0;
	};
}