#pragma once

#include<shapes/shape.h>

namespace schwi {
	class Rectangle :public Shape {
	private:
		Vector2d wh;

	public:
		Rectangle(double width, double height, const Frame* frame) :
			wh(width / 2, height / 2), Shape(frame) {}

		Rectangle(const Vector2d& wh, const Frame* frame) :
			wh(wh), Shape(frame) {}

		virtual bool Intersect(
			const Ray& r, SurfaceIntersection* out_isect
		)const override {
			Ray ray = frame->ToLocal(r);
			Normal3d normal{ 0,0,1 };
			if (std::abs(Dot(ray.direction(), normal)) < epsilon)return false;

			Vector3d op = Point3d(0, 0, 0) - ray.origin();
			double t = Dot(normal, op) / Dot(normal, ray.direction());

			if ((t > epsilon) && (t < ray.distance())) {
				Point3d p = ray(t);
				if (Abs(Vector2d(vec2<double>(p))) < wh) {
					r.set_distance(t);
					*out_isect = SurfaceIntersection(
						frame->ToWorld(p),
						Normal3d(frame->normal()),
						-r.direction(),
						Point2d(p.x/wh.x+.5,p.y/wh.y+.5),this);
					return true;
				}
			}
			return false;
		}

		virtual Bounds3d WorldBound() const override {
			Bounds3d b0(frame->ToWorld(Point3d(wh.x, wh.y, 0)), frame->ToWorld(Point3d(wh.x, -wh.y, 0)));
			Bounds3d b1(frame->ToWorld(Point3d(-wh.x, wh.y, 0)), frame->ToWorld(Point3d(-wh.x, -wh.y, 0)));
			return Union(b0,b1);
		}

		virtual double Area()const override {
			return 4 * wh.x * wh.y;
		}

		virtual SurfaceIntersection SamplePosition(
			const Vector2d& random, double* pdf
		)const override {
			SurfaceIntersection isect;
			isect.position = frame->ToWorld(Point3d(wh.x - 2 * wh.x * random[1], wh.y - 2 * wh.y * random[0],0));
			isect.normal = Normal3d(frame->normal());

			*pdf = 1 / Area();
			return isect;
		}
	};

	class Box :public Shape {
	private:
		ShapeList rectList;
		Vector3d p;

	public:
		Box(const Vector3d& p, const Frame* frame) :p(p), Shape(frame) {
			ShapeSPtr up = std::make_shared<Rectangle>(Vector2d(p.x, p.y), new Frame{ {1,0,0},{0,0,1},{0,-1,0},{0,p.z,0 } });
			ShapeSPtr down = std::make_shared<Rectangle>(Vector2d(p.x, p.y), new Frame{ {1,0,0},{0,0,-1},{0,1,0},{0,-p.z,0 } });
			ShapeSPtr left = std::make_shared<Rectangle>(Vector2d(p.y, p.z), new Frame{ {0,0,-1},{0,1,0},{1,0,0},{-p.x,0,0} });
			ShapeSPtr right = std::make_shared<Rectangle>(Vector2d(p.y, p.z), new Frame{ {0,0,1},{0,1,0},{-1,0,0},{p.x,0,0} });
			ShapeSPtr fore = std::make_shared<Rectangle>(Vector2d(p.x, p.z), new Frame{ {1,0,0},{0,1,0},{0,0,1},{0,0,p.y} });
			ShapeSPtr back = std::make_shared<Rectangle>(Vector2d(p.x, p.z), new Frame{ {1,0,0},{0,1,0},{0,0,1},{0,0,-p.y} });
			rectList = ShapeList{ up,down,left,right,fore,back };
		}

		virtual bool Intersect(
			const Ray& r, SurfaceIntersection* out_isect
		)const override {
			Ray ray = frame->ToLocal(r);

			bool hit = false;
			for (const auto& rect : rectList) {
				bool rect_hit = rect->Intersect(ray, out_isect);
				hit = hit ? hit : rect_hit;
			}
			if (hit) {
				r.set_distance(ray.distance());
				out_isect->position = frame->ToWorld(out_isect->position);
				out_isect->normal = frame->ToWorld(out_isect->normal);
				out_isect->wo = frame->ToWorld(out_isect->wo);
			}
			return hit;
		}

		virtual Bounds3d WorldBound() const override {
			const auto& position = frame->position();
			return Bounds3d(position - p, position + p);
		}

		virtual double Area()const override {
			return 8 * (p.x * p.y + p.y * p.z + p.z * p.x);
		}

		virtual SurfaceIntersection SamplePosition(
			const Vector2d& random, double* pdf
		)const override {
			*pdf = 1 / Area();
			std::cerr << "Box SamplePosition Error" << std::endl;
			return SurfaceIntersection();
		}
	};
}