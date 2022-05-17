#pragma once

// Global Include Files
#include<algorithm>
#include<iostream>
#include<fstream>
#include<memory>
#include<string>
#include<vector>
#include<initializer_list>
#include<typeinfo>
#include<omp.h>
#include<functional>
#include<chrono>

#include<AssetManager.h>
#include<math/math.h>
#include<math/matrix.h>
#include<math/vector2.h>
#include<math/vector3.h>
#include<math/point2.h>
#include<math/point3.h>
#include<math/normal3.h>
#include<math/RNG.h>

#pragma warning(disable : 4244)
#pragma warning(disable : 4996)

namespace schwi {

	// Global Manager
	static AssetManager<SchwiImage> imageManager;
	static AssetManager<Model> modelManager;

	// Global Forward Declarations
	template <typename T> class Bounds3;
	template <typename T> class Bounds2;
	class Color;
	class Ray;
	class Scene;
	class SurfaceIntersection;
	struct Primitive;
	class Frame;
	class Integrator;
	class PathIntegrator;
	class Camera;
	class PerspectiveCamera;
	struct CameraSample;
	class Shape;
	class Sampler;
	class BSDF;
	struct BSDFSample;
	class Material;
	class Light;
	class SchwiImage;
	class SchwiColor;
	template <typename T> class Texture;

	// Using
	using Bounds3d = Bounds3<double>;
	using Bounds2i = Bounds2<int>;

	using BsdfUPtr = std::unique_ptr<BSDF>;

	using MaterialSPtr = std::shared_ptr<Material>;
	using MaterialList = std::vector<MaterialSPtr>;

	using LightSPtr = std::shared_ptr<Light>;
	using LightList = std::vector<LightSPtr>;

	using ShapeSPtr = std::shared_ptr<Shape>;
	using ShapeList = std::vector<ShapeSPtr>;

	using PrimitiveList = std::vector<Primitive>;

	using TextureSPtr = std::shared_ptr<Texture<Color>>;
	using TextureList = std::vector<TextureSPtr>;

	inline BYTE GammaEncoding(double x) {
		return pow(Clamp(x), 1 / 2.2) * 255 + .5;
	}
}