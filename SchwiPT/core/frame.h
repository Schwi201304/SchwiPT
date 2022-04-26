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
		Frame(const Vector3d& s, const Vector3d& t, const Vector3d& n)
			:s(s.Normalize()), t(t.Normalize()), n(n.Normalize()) {}

		Frame(const Normal3d& n)
			:n(n.Normalize()) {
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
			return origin+Vector3d(Dot(s, world), Dot(t, world), Dot(n, world));
		}

		Point3d ToWorld(const Point3d& local)const {
			Vector3d l = local - origin;
			return Point3d(s * l.x + t * l.y + n * l.z);
		}

		//TODO:���ߣ����ߴ���
		Normal3d ToLocal(const Normal3d& world)const {
			return Normal3d(Dot(s, world), Dot(t, world), Dot(n, world));
		}

		Normal3d ToWorld(const Normal3d& local)const {
			return Normal3d(s * local.x + t * local.y + n * local.z);
		}

		Ray ToLocal(const Ray& ray)const {
			return Ray(ToLocal(ray.origin()), ToLocal(ray.direction()), ray.distance());
		}

		Ray ToWorld(const Ray& ray)const {
			return Ray(ToWorld(ray.origin()), ToWorld(ray.direction()), ray.distance());
		}

		const Vector3d& binormal()const { return s; }
		const Vector3d& tangent()const { return t; }
		const Vector3d& normal()const { return n; }

	private:
		void set_from_z() {
			Vector3d tmp = (std::abs(n.x) > 0.99) ? Vector3d(0, 1, 0) : Vector3d(1, 0, 0);
			t = Cross(n, tmp).Normalize();
			s = Cross(t, n).Normalize();
		}
	};
}