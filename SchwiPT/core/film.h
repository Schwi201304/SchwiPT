#pragma once

#include<core/schwi.h>
#include<core/color.h>
#include<core/bounds.h>
#include<filter/filter.h>
namespace schwi {
	class Film {
	private:
		std::unique_ptr<Color[]> pixels;
		static constexpr int filterTableWidth = 16;
		double filterTable[filterTableWidth * filterTableWidth];

	public:
		Point2i resolution;
		std::unique_ptr<Filter> filter;
		const std::string filename;

		Film(const Point2i& resolution, std::unique_ptr<Filter> filter, const std::string& filename) :
			resolution(resolution), filter(filter->Clone()), filename(filename) {
			pixels = std::unique_ptr <Color[]>(new Color[resolution.x * resolution.y]);
			int offset = 0;
			for (int y = 0; y < filterTableWidth; y++) {
				for (int x = 0; x < filterTableWidth; x++, offset++) {
					Point2d p;
					p.x = (x + 0.5) * filter->radius.x / filterTableWidth;
					p.y = (y + 0.5) * filter->radius.y / filterTableWidth;
					filterTable[offset] = filter->Evaluate(p);
				}
			}
		};

		Film(const std::string& filename, std::unique_ptr<Filter> filter) :
			filter(std::move(filter)), filename(filename) {
			ReadImage();
		}

		Color& GetPixel(const Point2i& p) {
			int offset = resolution.x * p.y + p.x;
			return pixels[offset];
		}

		Color& GetPixel(int x, int y) {
			auto [w, h] = resolution;
			x = x < 0 ? 0 : x;
			x = x >= w ? (w - 1) : x;
			y = y < 0 ? 0 : y;
			y = y >= h ? (h - 1) : y;
			int offset = w * y + x;
			return pixels[offset];
		}

		void WriteImage() {
			SchwiImage img(resolution.x, resolution.y, 3);
			for (int j = 0; j < resolution.y; j++) {
				for (int i = 0; i < resolution.x; i++) {
					img.setColor(i, j, ToByte(GetPixel(i, j)));
				}
			}
			img.write_file(filename, false);
		}

		Film Filter() {
			fprintf(stderr, "Gaussian Filter");
			Film ret(resolution, filter->Clone(), filename);
			auto [w, h] = resolution;

			double weight = 0;
			for (int r = -filterTableWidth + 1; r < filterTableWidth; r++)
				for (int c = -filterTableWidth + 1; c < filterTableWidth; c++)
					weight += filterTable[abs(r) * filterTableWidth + abs(c)];

			for (int hh = 0; hh < h; hh++) {
				for (int ww = 0; ww < w; ww++) {
					Color sum{};
					for (int r = -filterTableWidth + 1; r < filterTableWidth; r++) {
						for (int c = -filterTableWidth + 1; c < filterTableWidth; c++) {
							int offset = abs(r) * filterTableWidth + abs(c);
							sum = sum + GetPixel(ww + c, hh + r) * filterTable[offset];
						}
					}
					ret.GetPixel(ww, hh) = sum / weight;
				}
			}
			return ret;
		}

		void Clear() {
			auto [w, h] = resolution;
			for (int j = 0; j < h; j++)
				for (int i = 0; i < w; i++)
					GetPixel(i, j).Clear();
		}

	private:
		void ReadImage() {
			SchwiImage img(filename);
			auto [w, h] = img.resolution();
			resolution = Point2i(w, h);
			for (int i = 0; i < w * h; i++) {
				pixels[i] = ToColor(img[i]);
			}
		}
	};
}