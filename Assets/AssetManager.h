#pragma once

#include<utility>
#include<memory>
#include<image/image.h>

namespace schwi {
	using ImageSPtr = std::shared_ptr<SchwiImage>;
	using ImageList = std::vector<ImageSPtr>;

	class AssetManager {
	public:
		virtual ~AssetManager(){}
		AssetManager() = default;
	};
}