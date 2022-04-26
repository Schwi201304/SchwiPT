#pragma once

#include<core/schwi.h>
#include<material/material.h>
#include<core/primitive.h>
#include<shapes/shape.h>
#include<shapes/sphere.h>
#include<textures/imagemap.h>
#include<textures/constant.h>

namespace schwi {
	class Scene {
	public:
		Scene() = default;
		Scene(ShapeList shapeList, MaterialList materialList,
			LightList lightList, PrimitiveList primitiveList,
			TextureList textureList) :
			shapeList(shapeList),
			materialList(materialList),
			lightList(lightList),
			primitiveList(primitiveList),
			textureList(textureList) {}

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
			//{0,0,1}, {1,0,0 }, { 0,1,0 } {1,0,0}, {0,0,-1}, { 0,1,0 } {-1,0,0}, {0,0,1 }, { 0,1,0 }
			ShapeSPtr mirror = std::make_shared<Sphere>(Point3d(0, 0, 0), 16.5, new Frame{ {-1,0,0}, {0,0,1 }, { 0,1,0 }, {27, 16.5, -47 } });
			ShapeSPtr glass = std::make_shared<Sphere>(Point3d(73, 16.5, -78), 16.5);
			ShapeSPtr light = std::make_shared<Sphere>(Point3d(50, 681.6 - .27, -81.6), 600);
			ShapeList shapeList{ left, right, back, front, bottom, top, mirror, glass, light };

			ImageSPtr imgPtr = imageManager.Add("earthmap.png");
			std::shared_ptr<TextureFilter> bilinear = std::make_shared<BilinearFilter>();
			TextureSPtr texture =
				std::make_shared<ImageTexture<Color, Color>>(
					std::make_unique<UVMapping2D>(),
					bilinear, imgPtr);

			TextureSPtr redConst = std::make_shared<ConstantTexture<Color>>(Color(.75, .25, .25));
			TextureSPtr blueConst = std::make_shared<ConstantTexture<Color>>(Color(.25, .25, .75));
			TextureSPtr grayConst = std::make_shared<ConstantTexture<Color>>(Color(.75, .75, .75));
			TextureSPtr blackConst = std::make_shared<ConstantTexture<Color>>(Color(.0, .0, .0));
			TextureSPtr whiteConst = std::make_shared<ConstantTexture<Color>>(Color(1., 1., 1.));
			std::vector<TextureSPtr> textureList{ texture ,redConst,blueConst,grayConst,blackConst ,whiteConst };

			MaterialSPtr red = std::make_shared<Matte>(redConst);
			MaterialSPtr blue = std::make_shared<Matte>(blueConst);
			MaterialSPtr gray = std::make_shared<Matte>(grayConst);
			MaterialSPtr black = std::make_shared<Matte>(blackConst);
			MaterialSPtr earth = std::make_shared<Matte>(texture);

			MaterialSPtr mirror_mat = std::make_shared<Mirror>(redConst);
			MaterialSPtr plastic_mat = std::make_shared<Plastic>(blueConst, blackConst, 10);
			MaterialSPtr glass_mat = std::make_shared<Dielectric>(
				whiteConst, whiteConst, Fresnel::Glass);
			MaterialList materialList{ red, blue, gray, black,earth, mirror_mat, glass_mat, plastic_mat };



			std::shared_ptr<AreaLight> area_light = std::make_shared<AreaLight>(Color(10, 10, 10), light.get());
			LightList lightList{ area_light };


			std::vector<Primitive> primitiveList
			{
				{   left.get(),   red.get(),nullptr },
				{  right.get(),  blue.get(),nullptr },
				{   back.get(),  gray.get(),nullptr },
				{  front.get(), black.get(),nullptr },
				{ bottom.get(),  gray.get(),nullptr },
				{    top.get(),  gray.get(),nullptr },

				{ mirror.get(), earth.get(), nullptr },
				{  glass.get(),   glass_mat.get(), nullptr },

				{  light.get(), black.get(),area_light.get() },
			};

			return Scene{ shapeList, materialList, lightList, primitiveList ,textureList };
		}

	private:
		ShapeList shapeList;
		MaterialList materialList;
		LightList lightList;
		PrimitiveList primitiveList;
		TextureList textureList;
	};
}