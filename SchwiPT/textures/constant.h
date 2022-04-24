#pragma once

#include<textures/texture.h>

namespace schwi {
	template<typename T>
	class ConstantTexture :public Texture<T> {
	private:
		T value;

	public:
		ConstantTexture(const T& value):value(value){}

		virtual T Evaluate(const Intersection& isect)const override {
			return value;
		}
	};
}