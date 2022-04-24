#pragma once

#include"AssetManager.h"

namespace schwi {
	class ImageManager :public AssetManager {
	public:
		ImageManager(){}

		int Add(const std::string& filename) {
			std::string path ="img/"+filename;
			imgList.push_back(std::make_shared<SchwiImage>(filename));
			return imgList.size();
		}

		ImageSPtr GetImage(int index) {
			return imgList[index-1];
		}

	private:
		ImageList imgList;
	};
}