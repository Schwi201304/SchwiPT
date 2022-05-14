#pragma once

#include<core/schwi.h>
#include<math/normal3.h>
#include<core/ray.h>

namespace schwi {
	class Frame {
	private:
		Point3d origin{ 0,0,0 };
		Vector3d s{ 1,0,0 };
		Vector3d t{ 0,1,0 };
		Vector3d n{ 0,0,1 };
	public:
		Frame() = default;
		Frame(const Vector3d& s, const Vector3d& t, const Vector3d& n, const Point3d& origin = Point3d(0, 0, 0))
			:s(s.Normalize()), t(t.Normalize()), n(n.Normalize()), origin(origin) {}

		Frame(const Normal3d& n, const Point3d& origin = Point3d(0, 0, 0))
			:n(n.Normalize()), origin(origin) {
			set_from_z();
		}

	public:
		Vector3d ToLocal(const Vector3d& world)const {
			return Vector3d(Dot(s, world), Dot(t, world), Dot(n, world));
		}

		Vector3d ToWorld(const Vector3d& local)const {
			return s * local.x + t * local.y + n * local.z;
		}

		Point3d ToLocal(const Point3d& world)const {
			return Point3d(Dot(s, world - origin), Dot(t, world - origin), Dot(n, world - origin));
		}

		Point3d ToWorld(const Point3d& local)const {
			return Point3d(s * local.x + t * local.y + n * local.z) + Vector3d(origin);
		}

		//TODO: 法线，切线待定
		Normal3d ToLocal(const Normal3d& world)const {
			return Normal3d(Dot(s, world), Dot(t, world), Dot(n, world));
		}

		Normal3d ToWorld(const Normal3d& local)const {
			return Normal3d(s * local.x + t * local.y + n * local.z);
		}

		Ray ToLocal(const Ray& ray)const {
			return Ray(ToLocal(ray.origin()), ToLocal(ray.direction()).Normalize(), ray.distance());
		}

		Ray ToWorld(const Ray& ray)const {
			return Ray(ToWorld(ray.origin()), ToWorld(ray.direction()).Normalize(), ray.distance());
		}

		const Vector3d& binormal()const { return s; }
		const Vector3d& tangent()const { return t; }
		const Vector3d& normal()const { return n; }
		const Point3d& position()const { return origin; }

	private:
		void set_from_z() {
			Vector3d tmp = (std::abs(n.x) > 0.99) ? Vector3d(0, 1, 0) : Vector3d(1, 0, 0);
			t = Cross(n, tmp).Normalize();
			s = Cross(t, n).Normalize();
		}
	};
}