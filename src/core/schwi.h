#pragma once

// Global Include Files
#include<algorithm>
#include<assert.h>
#include<cmath>
#include<iostream>
#include<limits>
#include<memory>
#include<string>
#include<vector>
#include<initializer_list>
#include<typeinfo>
#include<omp.h>

#include<ImageManager.h>

#pragma warning(disable : 4244)
#pragma warning(disable : 4996)

namespace schwi {
	// Global Constants
	constexpr double Pi = 3.14159265358979323846;
	constexpr double InvPi = 0.31830988618379067154;
	constexpr double Inv2Pi = 0.15915494309189533577;
	constexpr double Inv4Pi = 0.07957747154594766788;
	constexpr double PiOver2 = 1.57079632679489661923;
	constexpr double PiOver4 = 0.78539816339744830961;
	constexpr double Sqrt2 = 1.41421356237309504880;
	constexpr double Log2 = 1.44269504088896322870;

	constexpr double Infinity = std::numeric_limits<double>::max();
	constexpr double MachineEpsilon = std::numeric_limits<double>::epsilon() * 0.5;

	constexpr int Dynamic = -1;

	// Global Manager
	static ImageManager imageManager;

	// Global Forward Declarations
	template <typename T> class vec2;
	template <typename T> class vec3;
	template <typename T> class vec4;
	template <typename T> class Vector2;
	template <typename T> class Vector3;
	template <typename T> class Point2;
	template <typename T> class Point3;
	template <typename T> class Normal3;
	template <typename T> class Bounds3;
	template<typename T, int R, int C>class Matrix;
	class Ray;
	class Scene;
	class Intersection;
	struct Primitive;
	class Frame;
	class Integrator;
	class PathIntegrator;
	class Camera;
	class PerspectiveCamera;
	struct CameraSample;
	class Shape;
	class Sphere;
	class Sampler;
	class RandomSampler;
	class BSDF;
	struct BSDFSample;
	class Lambertion;
	class Specular;
	class Fresnel;
	class Material;
	class Matte;
	class Mirror;
	class Dielectric;
	class Light;
	class AreaLight;
	class SchwiImage;
	class SchwiColor;
	template <typename T> class Texture;

	// Using
	using BYTE = unsigned char;
	using Degree = double;
	using Radian = double;

	using Bounds3d = Bounds3<double>;

	using Matrix2d = Matrix<double, 2, 2>;
	using Matrix3d = Matrix<double, 3, 3>;
	using Matrix4d = Matrix<double, 4, 4>;
	using MatrixXd = Matrix<double, Dynamic, Dynamic>;

	using Normal3d = Normal3<double>;

	using Point2d = Point2<double>;
	using Point2i = Point2<int>;

	using Point3d = Point3<double>;
	using Point3i = Point3<int>;

	using Vector2i = Vector2<int>;
	using Vector2d = Vector2<double>;

	using Vector3i = Vector3<int>;
	using Vector3d = Vector3<double>;

	using BsdfUPtr = std::unique_ptr<BSDF>;

	using MaterialSPtr = std::shared_ptr<Material>;
	using MaterialList = std::vector<MaterialSPtr>;

	using LightSPtr = std::shared_ptr<Light>;
	using LightList = std::vector<LightSPtr>;

	using ShapeSPtr = std::shared_ptr<Shape>;
	using ShapeList = std::vector<ShapeSPtr>;

	using PrimitiveList = std::vector<Primitive>;


	// Global Function
	inline Radian ToRadians(Degree deg) {
		return (Pi / 180) * deg;
	}

	inline Degree ToDegree(Radian rad) {
		return (180 * InvPi) * rad;
	}

	inline double Lerp(double t, double a, double b) {
		return (1 - t) * a + t * b;
	}

	inline double Clamp(double v, double l, double h) {
		return v > h ? h
			: v < l ? l
			: v;
	}

	inline double Clamp(double v) {
		return Clamp(v, 0, 1);
	}

	inline BYTE GammaEncoding(double x) {
		return pow(Clamp(x), 1 / 2.2) * 255 + .5;
	}
}