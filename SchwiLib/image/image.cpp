#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <3rdparty/stb_image/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <3rdparty/stb_image/stb_image_write.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <3rdparty/stb_image/stb_image_resize.h>

namespace schwi {

	SchwiColor SchwiColor::operator*(const double d) {
		SchwiColor ret = *this;
		for (int i = 0; i < comp; i++) {
			ret[i] *= d;
		}
		return ret;
	}

	bool SchwiImage::load_file(const std::string& filename, bool flip) {
		stbi_set_flip_vertically_on_load(flip);
		BYTE* data = stbi_load(filename.c_str(), &w, &h, &comp, 0);
		if (data) {
			pixels = std::vector<BYTE>(data, data + w * comp * h);
			stbi_image_free(data);
			return true;
		}
		else {
			std::cerr << "Error: fail to open image" << std::endl;
			return false;
		}
	}

	void SchwiImage::write_file(const std::string& filename, bool flip) const {
		stbi_flip_vertically_on_write(flip);
		stbi_write_png(filename.c_str(), w, h, comp, pixels.data(), 0);
	}

	void SchwiImage::setColor(const int x, const int y, const SchwiColor& color) {
		if (pixels.empty() || x < 0 || y < 0 || x >= w || y >= h) return;
		int ncmp = color.comp;
		int f = y * w * comp + x * comp;
		if (ncmp < comp) {
			for (int i = ncmp; i < comp; i++) {
				pixels[f + i] = 255;
			}
		}
		for (int i = 0; i < ncmp; i++) {
			pixels[f + i] = color[i];
		}
	}

	SchwiColor SchwiImage::getColor(const int flag)const {
		if (pixels.empty() || flag<0 || flag>w * h)
			return {};
		return SchwiColor(pixels.data() + flag, comp);
	}

	SchwiColor SchwiImage::getColor(const int x, const int y) const {
		if (pixels.empty() || x < 0 || y < 0 || x >= w || y >= h)
			return SchwiColor(0, 0, 0);
		int flag = y * w * comp + x * comp;
		return SchwiColor(pixels.data() + flag, comp);
	}

	SchwiColor SchwiImage::getColor(const double u, const double v) const {
		int x = u * (w - 1.);
		int y = v * (h - 1.);
		return getColor(x, y);
	}

	void SchwiImage::resize(const int width, const int height) {
		BYTE* data = new BYTE[width * height]();

		stbir_resize(pixels.data(), w, h, 0,
			data, width, height, 0,
			STBIR_TYPE_UINT8, comp, STBIR_ALPHA_CHANNEL_NONE, 0,
			STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP,
			STBIR_FILTER_BOX, STBIR_FILTER_BOX,
			STBIR_COLORSPACE_SRGB, nullptr
		);

		pixels.clear();
		std::vector<BYTE>().swap(pixels);
		pixels = std::vector<BYTE>(data, data + width * comp * height);
	}
}