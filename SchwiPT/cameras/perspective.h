#pragma once

#include<cameras/camera.h>

namespace schwi {
	class PerspectiveCamera :public Camera {
	private:
		Point3d position;
		Vector3d front;
		Vector3d right;
		Vector3d up;
		Vector2i resolution;
		double depth = 40;

	public:
		PerspectiveCamera(
			const Point3d& position, const Vector3d& front, const Vector3d& up,
			Degree fov, Vector2i resolution) :
			position(position),
			front(front),
			up(up),
			resolution(resolution) {
			double tan_fov = std::tan(ToRadians(fov) / 2);
			right = Cross(front, up).Normalize() * tan_fov * Aspect();
			this->up = Cross(right, front).Normalize() * tan_fov;
		}

		virtual Ray GenerateRay(const CameraSample& sample)const override {
			Vector3d direction = front
				+ right * (sample.SamplePoint.x / resolution.x - .5)
				+ up * (.5 - sample.SamplePoint.y / resolution.y);

			return Ray(position + direction * depth, direction.Normalize());
		}

	private:
		double Aspect() {
			return (double)resolution.x / resolution.y;
		}
	};
}