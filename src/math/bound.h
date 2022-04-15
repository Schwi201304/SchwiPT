#pragma once

#include "point3.h"

namespace schwi {
	template<typename T>
	class Bounds2 {};


	using Bounds3d = Bounds3<double>;
	template<typename T>
	class Bounds3 {
	public:
		Point3<T> pMin, pMax;
	public:
		Bounds3() {
			T minNum = std::numeric_limits<T>::lowest();
			T maxNum = std::numeric_limits<T>::max();
			pMin = Point3<T>(maxNum, maxNum, maxNum);
			pMax = Point3<T>(minNum, minNum, minNum);
		}
		Bounds3(const Point3<T>& p) : pMin(p), pMax(p) { }
		Bounds3(const Point3<T>& p1, const Point3<T>& p2)
			: pMin(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
				std::min(p1.z, p2.z)),
			pMax(std::max(p1.x, p2.x), std::max(p1.y, p2.y),
				std::max(p1.z, p2.z)) {	}

		const Point3<T>& operator[](int i) const {
			switch (i) {
			case 0:return Point3<T>(pMin.x, pMin.y, pMin.z);
			case 1:return Point3<T>(pMin.x, pMin.y, pMax.z);
			case 2:return Point3<T>(pMax.x, pMin.y, pMax.z);
			case 3:return Point3<T>(pMax.x, pMin.y, pMin.z);
			case 5:return Point3<T>(pMin.x, pMax.y, pMin.z);
			case 4:return Point3<T>(pMin.x, pMax.y, pMax.z);
			case 6:return Point3<T>(pMax.x, pMax.y, pMax.z);
			case 7:return Point3<T>(pMax.x, pMax.y, pMin.z);
			default:std::cerr << "Error(Bounds::[]:i<0||i>=8)" << std::endl;
			}
		}
		Point3<T>& operator[](int i) {
			switch (i) {
			case 0:return Point3<T>(pMin.x, pMin.y, pMin.z);
			case 1:return Point3<T>(pMin.x, pMin.y, pMax.z);
			case 2:return Point3<T>(pMax.x, pMin.y, pMax.z);
			case 3:return Point3<T>(pMax.x, pMin.y, pMin.z);
			case 5:return Point3<T>(pMin.x, pMax.y, pMin.z);
			case 4:return Point3<T>(pMin.x, pMax.y, pMax.z);
			case 6:return Point3<T>(pMax.x, pMax.y, pMax.z);
			case 7:return Point3<T>(pMax.x, pMax.y, pMin.z);
			default:std::cerr << "Error(Bounds::[]:i<0||i>=8)" << std::endl;
			}
		}

		Point3<T> Corner(int corner) const {
			return Point3<T>((*this)[(corner & 1)].x,
				(*this)[(corner & 2) ? 1 : 0].y,
				(*this)[(corner & 4) ? 1 : 0].z);
		}

		Vector3<T> Diagonal() const { return pMax - pMin; }

		T SurfaceArea() const {
			Vector3<T> d = Diagonal();
			return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
		}

		T Volume() const {
			Vector3<T> d = Diagonal();
			return d.x * d.y * d.z;
		}

		int MaximumExtent() const {
			Vector3<T> d = Diagonal();
			if (d.x > d.y && d.x > d.z)
				return 0;
			else if (d.y > d.z)
				return 1;
			else
				return 2;
		}

		Point3<T> Lerp(const Point3d& t) const {
			return Point3<T>(::Lerp(t.x, pMin.x, pMax.x),
				::Lerp(t.y, pMin.y, pMax.y),
				::Lerp(t.z, pMin.z, pMax.z));
		}

		Vector3<T> Offset(const Point3<T>& p) const {
			Vector3<T> o = p - pMin;
			if (pMax.x > pMin.x) o.x /= pMax.x - pMin.x;
			if (pMax.y > pMin.y) o.y /= pMax.y - pMin.y;
			if (pMax.z > pMin.z) o.z /= pMax.z - pMin.z;
			return o;
		}

		void BoundingSphere(Point3<T>* center, double* radius) const {
			*center = (pMin + pMax) / 2;
			*radius = Inside(*center, *this) ? Distance(*center, pMax) : 0;
		}

		template <typename T>
		inline bool IntersectP(const Ray& ray, double* hitt0, double* hitt1) const {
			double t0 = 0, t1 = ray.tMax;
			for (int i = 0; i < 3; ++i) {
				double invRayDir = 1 / ray.d[i];
				double tNear = (pMin[i] - ray.o[i]) * invRayDir;
				double tFar = (pMax[i] - ray.o[i]) * invRayDir;
				if (tNear > tFar) std::swap(tNear, tFar);
				tFar *= 1 + 2 * gamma(3);
				t0 = tNear > t0 ? tNear : t0;
				t1 = tFar < t1 ? tFar : t1;
				if (t0 > t1) return false;
			}
			if (hitt0) *hitt0 = t0;
			if (hitt1) *hitt1 = t1;
			return true;
		}

		template <typename T>
		inline bool IntersectP(const Ray& ray, const Vector3d& invDir, const int dirIsNeg[3]) const {
			const Bounds3d& bounds = *this;
			double tMin = (bounds[dirIsNeg[0]].x - ray.o.x) * invDir.x;
			double tMax = (bounds[1 - dirIsNeg[0]].x - ray.o.x) * invDir.x;
			double tyMin = (bounds[dirIsNeg[1]].y - ray.o.y) * invDir.y;
			double tyMax = (bounds[1 - dirIsNeg[1]].y - ray.o.y) * invDir.y;
			tMax *= 1 + 2 * gamma(3);
			tyMax *= 1 + 2 * gamma(3);

			if (tMin > tyMax || tyMin > tMax)
				return false;
			if (tyMin > tMin) tMin = tyMin;
			if (tyMax < tMax) tMax = tyMax;

			double tzMin = (bounds[dirIsNeg[2]].z - ray.o.z) * invDir.z;
			double tzMax = (bounds[1 - dirIsNeg[2]].z - ray.o.z) * invDir.z;
			tzMax *= 1 + 2 * gamma(3);

			if (tMin > tzMax || tzMin > tMax)
				return false;
			if (tzMin > tMin)
				tMin = tzMin;
			if (tzMax < tMax)
				tMax = tzMax;

			return (tMin < ray.tMax) && (tMax > 0);
		}
	};

	template <typename T>
	Bounds3<T>	Union(const Bounds3<T>& b, const Point3<T>& p) {
		return Bounds3<T>(Point3<T>(std::min(b.pMin.x, p.x),
			std::min(b.pMin.y, p.y),
			std::min(b.pMin.z, p.z)),
			Point3<T>(std::max(b.pMax.x, p.x),
				std::max(b.pMax.y, p.y),
				std::max(b.pMax.z, p.z)));
	}

	template <typename T>
	Bounds3<T> Union(const Bounds3<T>& b1, const Bounds3<T>& b2) {
		return Bounds3<T>(Point3<T>(std::min(b1.pMin.x, b2.pMin.x),
			std::min(b1.pMin.y, b2.pMin.y),
			std::min(b1.pMin.z, b2.pMin.z)),
			Point3<T>(std::max(b1.pMax.x, b2.pMax.x),
				std::max(b1.pMax.y, b2.pMax.y),
				std::max(b1.pMax.z, b2.pMax.z)));
	}

	template <typename T>
	Bounds3<T> Intersect(const Bounds3<T>& b1, const Bounds3<T>& b2) {
		return Bounds3<T>(Point3<T>(std::max(b1.pMin.x, b2.pMin.x),
			std::max(b1.pMin.y, b2.pMin.y),
			std::max(b1.pMin.z, b2.pMin.z)),
			Point3<T>(std::min(b1.pMax.x, b2.pMax.x),
				std::min(b1.pMax.y, b2.pMax.y),
				std::min(b1.pMax.z, b2.pMax.z)));
	}

	template <typename T>
	bool Overlaps(const Bounds3<T>& b1, const Bounds3<T>& b2) {
		bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
		bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
		bool z = (b1.pMax.z >= b2.pMin.z) && (b1.pMin.z <= b2.pMax.z);
		return (x && y && z);
	}

	template <typename T>
	bool Inside(const Point3<T>& p, const Bounds3<T>& b) {
		return (p.x >= b.pMin.x && p.x <= b.pMax.x &&
			p.y >= b.pMin.y && p.y <= b.pMax.y &&
			p.z >= b.pMin.z && p.z <= b.pMax.z);
	}

	template <typename T>
	bool InsideExclusive(const Point3<T>& p, const Bounds3<T>& b) {
		return (p.x >= b.pMin.x && p.x < b.pMax.x&&
			p.y >= b.pMin.y && p.y < b.pMax.y&&
			p.z >= b.pMin.z && p.z < b.pMax.z);
	}

	template <typename T, typename U>
	inline Bounds3<T> Expand(const Bounds3<T>& b, U delta) {
		return Bounds3<T>(b.pMin - Vector3<T>(delta, delta, delta),
			b.pMax + Vector3<T>(delta, delta, delta));
	}
}