#pragma once

#include<core/schwi.h>
#include<material/material.h>
#include<core/primitive.h>
#include<shapes/shape.h>
#include<shapes/sphere.h>
#include<textures/imagemap.h>

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

			MaterialSPtr mirror_mat = std::make_shared<Mirror>(Color(.75, .25, .25));
			MaterialSPtr plastic_mat = std::make_shared<Plastic>(Color(.25, .25, .75), Color(.0, .0, .0), 100);
			MaterialSPtr glass_mat = std::make_shared<Dielectric>(
				Color(1, 1, 1), Color(1, 1, 1), Fresnel::Glass);
			MaterialList materialList{ red, blue, gray, black, mirror_mat, glass_mat,plastic_mat };

			//int index=imageManager.Add("earthmap.png");
			//std::shared_ptr<TextureFilter> bilinear= std::make_shared<BilinearFilter>();
			//
			//std::shared_ptr<Texture<Color>> texture =
			//	std::make_shared<ImageTexture<Color, Color>>(
			//		std::make_unique<UVMapping2D>(),
			//		bilinear, imageManager.GetImage(index)
			//		);

			std::shared_ptr<AreaLight> area_light = std::make_shared<AreaLight>(Color(10, 10, 10), light.get());
			LightList lightList{ area_light };


			std::vector<Primitive> primitiveList
			{
				{   left.get(),   red.get(), nullptr,nullptr },
				{  right.get(),  blue.get(), nullptr,nullptr },
				{   back.get(),  gray.get(), nullptr,nullptr },
				{  front.get(), black.get(), nullptr,nullptr },
				{ bottom.get(),  gray.get(), nullptr,nullptr },
				{    top.get(),  gray.get(), nullptr,nullptr },

				{ mirror.get(), blue.get(),nullptr, nullptr },
				{  glass.get(),   glass_mat.get(),nullptr, nullptr },

				{  light.get(), black.get(),nullptr, area_light.get() },
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