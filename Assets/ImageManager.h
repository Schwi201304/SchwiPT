#pragma once

#include"AssetManager.h"

namespace schwi {
	class ImageManager :public AssetManager {
	public:
		ImageManager(){}

		ImageSPtr Add(const std::string& filename);
		ImageSPtr Get(const std::string& filename);

	private:
		ImageMap imgMap;
	};
}