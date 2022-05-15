#pragma once

#include<core/schwi.h>

namespace schwi {

	inline Point2d SampleDiskUniform(const Vector2d& random) {
		double r = sqrt(random[0]);
		double theta = 2 * Pi * random[1];
		return Point2d(r * cos(theta), r * sin(theta));
	}

	inline Point2d SampleDiskConcentric(const Vector2d& random) {
		Vector2d uOffset = 2. * random - Vector2d(1, 1);

		if (uOffset.x == 0 && uOffset.y == 0)
			return Point2d(0, 0);

		double theta, r;
		if (abs(uOffset.x) > abs(uOffset.y)) {
			r = uOffset.x;
			theta = PiOver4 * (uOffset.y / uOffset.x);
		}
		else {
			r = uOffset.y;
			theta = PiOver2 - PiOver4 * (uOffset.y / uOffset.x);
		}

		return Point2d(r * cos(theta), r * sin(theta));
	}

	inline Vector3d SampleHemisphereCosine(const Vector2d& random) {
		Point2d d = SampleDiskConcentric(random);
		double z = sqrt(max(0., 1 - d.x * d.x - d.y * d.y));
		//double z = sqrt(1 - random[0]);
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
		double r = sqrt(max(0., 1 - z * z));
		double phi = 2 * Pi * random[1];

		return Vector3d(r * cos(phi), r * sin(phi), z);
	}

	inline Vector3d SampleSphereUniform(const Vector2d& random) {
		double z = 1 - 2 * random[0];
		double r = sqrt(max(0., 1 - z * z));
		double phi = 2 * Pi * random[1];

		return Vector3d(r * cos(phi), r * sin(phi), z);
	}

	inline double PdfSphereUniform() {
		return Inv4Pi;
	}

	Vector3d SampleConeUniform(const Vector2d& random, double cos_theta_max)
	{
		double cos_theta = (1 - random[0]) + random[0] * cos_theta_max;
		double sin_theta = sqrt(1 - cos_theta * cos_theta);
		double phi = random[1] * 2 * Pi;

		return Vector3d(
			cos(phi) * sin_theta,
			sin(phi) * sin_theta,
			cos_theta);
	}

	double PdfConeUniform(double cos_theta_max)
	{
		return 1 / (2 * Pi * (1 - cos_theta_max));
	}

	Point2d SampleTriangleUniform(const Vector2d& random)
	{
		double su0 = std::sqrt(random[0]);
		return Point2d(1 - su0, random[1] * su0);
	}


	inline double PowerHeuristic(int nf, double fPdf, int ng, double gPdf)
	{
		double f = nf * fPdf;
		double g = ng * gPdf;
		return (f * f) / (f * f + g * g);
	}
}