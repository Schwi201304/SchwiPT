#include"ImageManager.h"

namespace schwi {
	ImageSPtr ImageManager::Add(const std::string& filename) {
		std::string path = "img/" + filename;
		auto imagePtr = std::make_shared<SchwiImage>(path);
		imgMap.emplace(filename, imagePtr);
		return imagePtr;
	}

	ImageSPtr ImageManager::Get(const std::string& filename) {
		return imgMap[filename];
	}
}