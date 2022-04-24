#pragma once

#include<utility>
#include<memory>
#include<image/image.h>
#include<map>

namespace schwi {
	using ImageSPtr = std::shared_ptr<SchwiImage>;
	using ImageMap = std::map<std::string, ImageSPtr>;

	class AssetManager {
	public:
		virtual ~AssetManager(){}
		AssetManager() = default;
	};
}