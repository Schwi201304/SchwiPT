#pragma once

#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<utility>

#pragma warning(disable : 4244)
#pragma warning(disable : 26451)

namespace schwi {
	using BYTE = unsigned char;
	using Resolution = std::pair<int, int>;

	class SchwiColor {
	public:
		BYTE rgba[4] = { 0,0,0,0 };
		BYTE comp;

		SchwiColor() :rgba{ 0,0,0,0 }, comp(3){};//default
		SchwiColor(BYTE r) :rgba{ r,255,255,255 }, comp(1){}//Grayscale
		SchwiColor(BYTE r, BYTE g, BYTE b) :rgba{ r,g,b,255 }, comp(3) {}//rgb
		SchwiColor(BYTE r, BYTE g, BYTE b, BYTE a) :rgba{ r,g,b,a }, comp(4) {}//rgba
		SchwiColor(const BYTE* color, int _comp) :comp(_comp) {
			for (int i = _comp; i--; rgba[i] = color[i]);
		}

		BYTE& operator[](const int i) { return rgba[i]; }
		const BYTE& operator[](const int i)const { return rgba[i]; }
		SchwiColor operator*(const double d);
	};

	class SchwiImage {
	private:
		int w, h, comp;
		std::vector<BYTE> pixels;

	public:
		enum Format { GRAY = 1, RGB = 3, RGBA = 4 };
		SchwiImage() = default;
		SchwiImage(const std::string& filename) :w(0), h(0), comp(0) {
			load_file(filename);
		}
		SchwiImage(int _w, int _h, int _comp = 3) :w(_w), h(_h), comp(_comp) {
			pixels = std::vector<BYTE>(w * comp * h);
		}

		inline BYTE* data() { return pixels.data(); }
		inline int width()const { return w; }
		inline int height()const { return h; }
		inline int channels() const { return comp; }
		inline bool empty()const { return pixels.empty(); }
		inline Resolution resolution() {
			return Resolution{ width(),height() };
		}
		inline void info(const std::string& filename) {
			std::cerr << "SchwiImage: [" << filename << "]: # w# " << width() << " h# " << height() << " comp# " << channels() << std::endl;
		}

		bool load_file(const std::string& filename, bool flip = true);
		void write_file(const std::string& filename, bool flip = true)const;
		void setColor(const int x, const int y, const SchwiColor& color);
		SchwiColor getColor(const int x, const int y) const;
		SchwiColor getColor(const double u, const double v)const;
		void resize(const int width, const int height);
	};
}
