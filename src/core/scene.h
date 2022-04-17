#pragma once

#include<core/schwi.h>
#include<material/material.h>
#include<core/primitive.h>
#include<shape/shape.h>
#include<shape/sphere.h>

namespace schwi {
	class Scene {
	public:
		Scene() = default;
		Scene(ShapeList shapeList, MaterialList materialList,
			LightList lightList, PrimitiveList primitiveList) :
			shapeList(shapeList),
			materialList(materialList),
			lightList(lightList),
			primitiveList(primitiveList) {}

	public:
		bool Intersect(Ray& ray, Intersection* isect)const {
			bool hit = false;

			for (const auto& primitive : primitiveList) {
				if (primitive.Intersect(ray, isect)) {
					hit = true;
				}
			}

			return hit;
		}

	public:
		static Scene CreateSmallPTScene() {
			ShapeSPtr left = std::make_shared<Sphere>(Point3d(1e5 + 1, 40.8, -81.6), 1e5);
			ShapeSPtr right = std::make_shared<Sphere>(Point3d(-1e5 + 99, 40.8, -81.6), 1e5);
			ShapeSPtr back = std::make_shared<Sphere>(Point3d(50, 40.8, -1e5), 1e5);
			ShapeSPtr front = std::make_shared<Sphere>(Point3d(50, 40.8, 1e5 - 170), 1e5);
			ShapeSPtr bottom = std::make_shared<Sphere>(Point3d(50, 1e5, -81.6), 1e5);
			ShapeSPtr top = std::make_shared<Sphere>(Point3d(50, -1e5 + 81.6, -81.6), 1e5);

			ShapeSPtr mirror = std::make_shared<Sphere>(Point3d(27, 16.5, -47), 16.5);
			ShapeSPtr glass = std::make_shared<Sphere>(Point3d(73, 16.5, -78), 16.5);
			ShapeSPtr light = std::make_shared<Sphere>(Point3d(50, 681.6 - .27, -81.6), 600);
			ShapeList shapeList{ left, right, back, front, bottom, top, mirror, glass, light };


			MaterialSPtr red = std::make_shared<Matte>(Color(.75, .25, .25));
			MaterialSPtr blue = std::make_shared<Matte>(Color(.25, .25, .75));
			MaterialSPtr gray = std::make_shared<Matte>(Color(.75, .75, .75));
			MaterialSPtr black = std::make_shared<Matte>(Color());

			MaterialSPtr mirror_mat = std::make_shared<Mirror>(Color(1, 1, 1) * 0.999);
			MaterialSPtr glass_mat = std::make_shared<Glass>(Color(1, 1, 1) * 0.999, Color(1, 1, 1) * 0.999, 1.5);
			MaterialList materialList{ red, blue, gray, black, mirror_mat, glass_mat };


			std::shared_ptr<AreaLight> area_light = std::make_shared<AreaLight>(Color(12, 12, 12), light.get());
			LightList lightList{ area_light };


			std::vector<Primitive> primitiveList
			{
				{   left.get(),   red.get(), nullptr},
				{  right.get(),  blue.get(), nullptr },
				{   back.get(),  gray.get(), nullptr },
				{  front.get(), black.get(), nullptr },
				{ bottom.get(),  gray.get(), nullptr },
				{    top.get(),  gray.get(), nullptr },

				{ mirror.get(), mirror_mat.get(), nullptr },
				{glass.get(),  glass_mat.get(), nullptr},

				{  light.get(), black.get(), area_light.get() },
			};

			return Scene{ shapeList, materialList, lightList, primitiveList };
		}

	private:
		ShapeList shapeList;
		MaterialList materialList;
		LightList lightList;
		PrimitiveList primitiveList;
	};
}