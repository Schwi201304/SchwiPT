#pragma once

#include<core/schwi.h>
#include<core/bounds.h>
#include<fliter/fliter.h>
namespace schwi {
	class Film {
	private:
        struct Pixel {
            double xyz[3];
            Pixel() { xyz[0] = xyz[1] = xyz[2]= 0; }
        };
        std::unique_ptr<Pixel[]> pixels;
        static constexpr int filterTableWidth = 16;
        double filterTable[filterTableWidth * filterTableWidth];

        Pixel& GetPixel(const Point2i& p){}

    public:
        const Point2i resolution;
        std::unique_ptr<Filter> filter;
        const std::string filename;

        Film(const Point2i& resolution,
            std::unique_ptr<Filter> filter, 
            const std::string& filename);
        Bounds2i GetSampleBounds() const;
        void WriteImage(double splatScale = 1);
        void Clear();
	};
}