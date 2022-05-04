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
			const Ray& r, Intersection* out_isect
		)const override {
			Ray ray = frame->ToLocal(r);
			Normal3d normal{ 0,0,1 };
			if (std::abs(Dot(ray.direction(), normal)) < epsilon)return false;

			Vector3d op = Point3d(0, 0, 0) - ray.origin();
			double t = Dot(normal, op) / Dot(normal, ray.direction());

			if ((t > epsilon) && (t < ray.distance())) {
				Point3d hit_point = ray(t);
				if (Abs(Vector2d(vec2<double>(hit_point))) < wh) {
					r.set_distance(t);
					*out_isect = Intersection(
						frame->ToWorld(hit_point),
						Normal3d(frame->normal()),
						-r.direction(),
						Point2d(.5, .5));
					return true;
				}
			}
		}
	};

	class Box :public Shape {
	private:
		ShapeList rectList;

	public:
		Box(const Vector3d& p, const Frame* frame) :Shape(frame) {
			ShapeSPtr up = std::make_shared<Rectangle>(Vector2d(p.x, p.y), new Frame{ {1,0,0},{0,0,1},{0,-1,0},{0,p.z,0 } });
			ShapeSPtr down = std::make_shared<Rectangle>(Vector2d(p.x, p.y), new Frame{ {1,0,0},{0,0,-1},{0,1,0},{0,-p.z,0 } });
			ShapeSPtr left = std::make_shared<Rectangle>(Vector2d(p.y, p.z), new Frame{ {0,0,-1},{0,1,0},{1,0,0},{-p.x,0,0} });
			ShapeSPtr right = std::make_shared<Rectangle>(Vector2d(p.y, p.z), new Frame{ {0,0,1},{0,1,0},{-1,0,0},{p.x,0,0} });
			ShapeSPtr fore = std::make_shared<Rectangle>(Vector2d(p.x, p.z), new Frame{ {1,0,0},{0,1,0},{0,0,1},{0,0,p.y} });
			ShapeSPtr back = std::make_shared<Rectangle>(Vector2d(p.x, p.z), new Frame{ {1,0,0},{0,1,0},{0,0,1},{0,0,-p.y} });
			rectList = ShapeList{ up,down,left,right,fore,back };
		}

		virtual bool Intersect(
			const Ray& r, Intersection* out_isect
		)const override {
			Ray ray = frame->ToLocal(r);

			bool hit = false;
			for (auto rect : rectList) {
				bool rect_hit = rect->Intersect(ray, out_isect);
				hit = hit ? hit : rect_hit;
			}
			r.set_distance(ray.distance());
			out_isect->position = frame->ToWorld(out_isect->position);
			out_isect->normal = frame->ToWorld(out_isect->normal);
			out_isect->wo = frame->ToWorld(out_isect->wo);
			return hit;
		}
	};
}