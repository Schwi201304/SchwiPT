#pragma once

#include<core/schwi.h>
#include<math/normal3.h>

namespace schwi {
	class Frame {
	private:
		Vector3d s{ 1,0,0 };
		Vector3d t{ 0,1,0 };
		Vector3d n{ 0,0,1 };
	public:
		Frame(const Vector3d& s, const Vector3d& t, const Vector3d& n)
			:s(s.Normalize()), t(t.Normalize()), n(n.Normalize()) {}

		Frame(const Vector3d& n)
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