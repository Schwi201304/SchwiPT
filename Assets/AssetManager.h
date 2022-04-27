#pragma once

#include<utility>
#include<memory>
#include<map>
#include<type_traits>
#include<image/image.h>
#include<model/objloader.h>

namespace schwi {
	using ImageSPtr = std::shared_ptr<SchwiImage>;
	using ImageMap = std::map<std::string, ImageSPtr>;

	using ModelSPtr = std::shared_ptr<Model>;
	using ModelMap = std::map<std::string, ModelSPtr>;

	template<typename T>
	class AssetManager {
	private:
		std::map<std::string, std::shared_ptr<T>> map;

	public:
		~AssetManager(){}
		AssetManager() = default;

		auto Add(const std::string & filename) {
			std::string prefix = TypeCheck();
			std::string path = prefix + filename;
			auto Ptr = std::make_shared<T>(path);
			Ptr->info(filename);
			map.emplace(filename, Ptr);
			return Ptr;
		}

		auto Get(const std::string & filename) {
			return map[filename];
		}

	private:
		std::string TypeCheck() {
			if (std::is_same<T, SchwiImage>::value)
				return "Assets/img/";
			else if (std::is_same<T, Model>::value)
				return "Assets/obj/";
		}
	};
}