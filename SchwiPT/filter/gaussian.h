#pragma once

#include<filter/filter.h>

namespace schwi {
	class GaussianFilter :public Filter {
	private:
		const double alpha;
		const double expX, expY;

		double Gaussian(double d,double expv)const{
			return max(0., exp(-alpha * d * d) - expv);
		}

	public:
		GaussianFilter(const Vector2d& radius,double alpha):
			Filter(radius),
			alpha(alpha),
			expX(exp(-alpha*radius.x*radius.x)),
			expY(exp(-alpha*radius.y*radius.y)){}

		double Evaluate(const Point2d& p)const {
			return Gaussian(p.x, expX) * Gaussian(p.y, expY);
		}

		std::unique_ptr<Filter> Clone()const override{
			return std::make_unique<GaussianFilter>(radius, alpha);
		}
	};
}