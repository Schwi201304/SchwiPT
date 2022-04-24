#pragma once

#include<textures/texture.h>

namespace schwi {
	template<typename Tmemory, typename Treturn>
	class ImageTexture :public Texture<Treturn> {
	private:
		std::shared_ptr<SchwiImage> img;
		std::unique_ptr<TextureMapping2D> mapping;
		std::shared_ptr<TextureFilter> filter;

	public:
		ImageTexture(std::unique_ptr<TextureMapping2D> mapping,
			std::shared_ptr<TextureFilter> filter,
			std::shared_ptr<SchwiImage> img)
			:mapping(std::move(mapping)),
			filter(filter),
			img(img) {}

		virtual Treturn Evaluate(const Intersection& isect) const override {
			Point2d point=mapping->Map(isect);
			Color color= filter->Filter(img, point);
			return color;
		}
	};
}