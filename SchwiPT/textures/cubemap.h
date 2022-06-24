#pragma once

#include<core/schwi.h>
#include<textures/texture.h>

namespace schwi {
	class CubeMap {
	private:
		std::shared_ptr<SchwiImage> img;
		std::vector<Vector2d> offset = {
			{2. / 4.,1. / 3.},	// 0 +x
			{0.,1. / 3.},		// 1 -x
			{1. / 4.,2. / 3.},	// 2 +y
			{1. / 4.,0.},		// 3 -y
			{1. / 4.,1. / 3.},	// 4 +z
			{3. / 4.,1. / 3.}	// 5 -z
		};

	public:
		CubeMap(std::shared_ptr<SchwiImage> img) :
			img(img) {}

		Color Evaluate(const Vector3d& v) {
			Point2d uv;
			int md = MaxDimension(Abs(v));
			int xyz = md + 1;
			if (v[md] < 0)
				xyz = -xyz;
			switch (xyz)
			{
			case 1://+x
				uv = Map(0, 2, 1, v) + offset[0]; break;
			case 2://+y
				uv = Map(1, 0, 2, v) + offset[2]; break;
			case 3://+z
				uv = Map(2, 0, 1, v) + offset[4]; break;
			case -1://-x
				uv = Map(0, 2, 1, v) + offset[1]; break;
			case -2://-y
				uv = Map(1, 0, 2, v) + offset[3]; break;
			case -3://-z
				uv = Map(2, 0, 1, v) + offset[5]; break;
			default:
				uv = Point2d(0, 0);
				std::cerr << "Error: CubeMap " << v << std::endl;
			}

			//return Color(v[0] * 0.5 + 0.5, v[1] * 0.5 + 0.5, v[2] * 0.5 + 0.5);
			//return Color(uv[0], uv[1], 0);
			return BilinearFilter().Filter(img, uv);
		}

	private:
		Point2d Map(int z, int x, int y, const Vector3d& v) {
			double inv = abs(v[z]);
			return Point2d((v[x] /inv + 1.) / 8., (v[y] / inv + 1.) / 6.);
		}
	};
}