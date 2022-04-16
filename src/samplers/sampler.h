#pragma once

#include<core/schwi.h>
#include<math/RNG.h>
#include<math/point2.h>

namespace schwi {

	struct CameraSample
	{
		Point2d SamplePoint;
	};

	class Sampler {
	protected:
		RNG rng;
		int SamplePerPixel;
		int CurrentSampleIndex;

	public:
		Sampler(int spp) :SamplePerPixel(spp) {}

		virtual int GetSPP() {
			return SamplePerPixel;
		}

		virtual void SetSPP(int spp) {
			SamplePerPixel = spp;
		}

		virtual std::unique_ptr<Sampler> clone() = 0;

		virtual void StartPixel() {
			CurrentSampleIndex = 0;
		}

		virtual bool NextSample(){
			CurrentSampleIndex += 1;
			return CurrentSampleIndex < SamplePerPixel;
		}

	public:
		virtual double GetDouble() = 0;
		virtual Vector2d GetVector2d() = 0;
		virtual CameraSample GetCameraSample(Point2d p) = 0;
	};

	inline Point2d SampleDiskUniform(const Vector2d& random) {
		double r = std::sqrt(random[0]);
		double theta = 2 * Pi * random[1];
		return Point2d(r * std::cos(theta), r * std::sin(theta));
	}

	inline Point2d SampleDiskConcentric(const Vector2d& random) {
		Vector2d uOffset = Vector2d(2. * random - Vector2d(1, 1));

		if (uOffset.x == 0 && uOffset.y == 0)
			return Point2d(0, 0);

		double theta, r;
		if (std::abs(uOffset.x) > std::abs(uOffset.y)) {
			r = uOffset.x;
			theta = Inv4Pi * (uOffset.y / uOffset.x);
		}
		else {
			r = uOffset.y;
			theta = Inv2Pi - Inv4Pi * (uOffset.y / uOffset.x);
		}

		return Point2d(r * Vector2d(std::cos(theta), std::sin(theta)));
	}

	inline Vector3d SampleHemisphereCosine(const Vector2d& random) {
		Point2d d = SampleDiskConcentric(random);
		double z = std::sqrt(std::max(0., 1 - d.x * d.x - d.y * d.y));
		return Vector3d(d.x, d.y, z);
	}

	inline double PdfHemisphereCosine(double cos_theta) {
		return cos_theta * InvPi;
	}

	inline double PdfHemisphereUniform() {
		return Inv2Pi;
	}

	inline Vector3d SampleHemisphereUniform(const Vector2d& random) {
		double z = random[0];
		double r = std::sqrt(std::max(0., 1 - z * z));
		double phi = 2 * Pi * random[1];

		return Vector3d(r * std::cos(phi), r * std::sin(phi), z);
	}

	inline Vector3d SampleSphereUniform(const Vector2d& random) {
		double z = 1 - 2 * random[0];
		double r = std::sqrt(std::max(0., 1 - z * z));
		double phi = 2 * Pi * random[1];

		return Vector3d(r * std::cos(phi), r * std::sin(phi), z);
	}

	inline double PdfSphereUniform() {
		return Inv4Pi;
	}

	Vector3d SampleConeUniform(const Vector2d& random, double cos_theta_max)
	{
		double cos_theta = (1 - random[0]) + random[0] * cos_theta_max;
		double sin_theta = std::sqrt(1 - cos_theta * cos_theta);
		double phi = random[1] * 2 * Pi;

		return Vector3d(
			std::cos(phi) * sin_theta,
			std::sin(phi) * sin_theta,
			cos_theta);
	}

	double PdfConeUniform(double cos_theta_max)
	{
		return 1 / (2 * Pi * (1 - cos_theta_max));
	}
}