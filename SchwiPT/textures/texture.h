#pragma once

#include<core/schwi.h>
#include<core/intersection.h>

namespace schwi {
	template <typename T>
	class Texture {
	public:
		virtual ~Texture() {}
		virtual T Evaluate(const SurfaceIntersection& isect) const = 0;
	};

	class TextureMapping2D {
	public:
		virtual ~TextureMapping2D() {}
		virtual Point2d Map(const SurfaceIntersection& isect)const = 0;
	};

	class UVMapping2D :public TextureMapping2D {
	private:
		const double su, sv, du, dv;

	public:
		UVMapping2D(double su = 1, double sv = 1, double du = 0, double dv = 0)
			: su(su), sv(sv), du(du), dv(dv) {}
		virtual Point2d Map(const SurfaceIntersection& isect)const override {
			return Point2d(su * isect.uv[0] + du, sv * isect.uv[1] + dv);
		}
	};

	class TextureFilter {
	public:
		virtual ~TextureFilter() {}
		virtual Color Filter(std::shared_ptr<SchwiImage> img, const Point2d& point)const = 0;
	};

	class NearestFilter :public TextureFilter {
	public:
		NearestFilter() = default;
		virtual Color Filter(std::shared_ptr<SchwiImage> img, const Point2d& point)const override {
			auto [w, h] = img->resolution();

			int u = std::round((w - 1.) * point.x);
			int v = std::round((h - 1.) * point.y);

			return ToColor(img->getColor(u, v));
		}
	};

	class BilinearFilter :public TextureFilter {
	public:
		BilinearFilter() = default;
		virtual Color Filter(std::shared_ptr<SchwiImage> img, const Point2d& point)const override {
			auto [w, h] = img->resolution();

			double u = (w - 1.) * point.x;
			double v = (h - 1.) * point.y;
			int x_min = std::floor(u);
			int x_max = std::ceil(u);
			int y_min = std::floor(v);
			int y_max = std::ceil(v);
			double tx = u - x_min;
			double ty = v - y_min;

			Color lt = ToColor(img->getColor(x_min,y_max));
			Color lb = ToColor(img->getColor(x_min,y_min));
			Color rt = ToColor(img->getColor(x_max,y_max));
			Color rb = ToColor(img->getColor(x_max,y_min));

			return Lerp(ty,
				Lerp(tx, lb, rb),
				Lerp(tx, lt, rt));
		}
	};
}