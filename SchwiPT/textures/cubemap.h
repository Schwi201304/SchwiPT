#pragma once

#include<core/schwi.h>
#include<textures/texture.h>

namespace schwi {
	class CubeMap {
		/**
		*	 +Y
		* -X +Z +X -Z
		*    -Y
		* 常见CubeMap符合以上格式，基于左手系
		* 渲染器是右手系
		* 左右手系互转时应将+X与-X的贴图对调，并将所有贴图x方向取反
		* 这里暂未实现
		*/
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
				uv = Map(v[0], -v[2], v[1]) + offset[0]; break;
			case 2://+y
				uv = Map(v[1], v[0], -v[2]) + offset[2]; break;
			case 3://+z
				uv = Map(v[2], v[0], v[1]) + offset[4]; break;
			case -1://-x
				uv = Map(v[0], v[2], v[1]) + offset[1]; break;
			case -2://-y
				uv = Map(v[1], v[0], v[2]) + offset[3]; break;
			case -3://-z
				uv = Map(v[2], -v[0], v[1]) + offset[5]; break;
			default:
				uv = Point2d(0, 0);
				std::cerr << "Error: CubeMap " << v << std::endl;
			}

			//return Color(v[0] * 0.5 + 0.5, v[1] * 0.5 + 0.5, v[2] * 0.5 + 0.5);
			//return Color(uv[0], uv[1], 0);
			return BilinearFilter().Filter(img, uv);
		}

	private:
		Point2d Map(double z, double x, double y) {
			double inv = abs(z);
			return Point2d((x /inv + 1.) / 8., (y / inv + 1.) / 6.);
		}
	};
}