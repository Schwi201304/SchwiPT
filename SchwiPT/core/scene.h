#pragma once

#include<core/schwi.h>
#include<material/material.h>
#include<core/primitive.h>
#include<shapes/shape.h>
#include<shapes/sphere.h>
#include<shapes/triangle.h>
#include<shapes/disk.h>
#include<shapes/rectangle.h>
#include<shapes/cylinder.h>
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
			ShapeSPtr left = std::make_shared<Sphere>(Point3d(-1e5 - 40, 0, 0), 1e5, new Frame());
			ShapeSPtr right = std::make_shared<Sphere>(Point3d(1e5 + 40, 0, 0), 1e5, new Frame());
			ShapeSPtr back = std::make_shared<Sphere>(Point3d(0, 0, -1e5 - 50), 1e5, new Frame());
			ShapeSPtr front = std::make_shared<Sphere>(Point3d(0, 0, 1e5 + 50), 1e5, new Frame());
			ShapeSPtr bottom = std::make_shared<Sphere>(Point3d(0, -1e5 - 40, 0), 1e5, new Frame());
			ShapeSPtr top = std::make_shared<Sphere>(Point3d(0, 1e5 + 40, 0), 1e5, new Frame());

			//{0,0,1}, {1,0,0 }, { 0,1,0 } {1,0,0}, {0,0,-1}, { 0,1,0 } {-1,0,0}, {0,0,1 }, { 0,1,0 }
			ShapeSPtr mirror = std::make_shared<Sphere>(Point3d(0, 0, 0), 16.5, new Frame{ {-1,0,0}, {0,0,1 }, { 0,1,0 }, {20, -40 + 16.5, -25 } });
			ShapeSPtr glass = std::make_shared<Sphere>(Point3d(-20, -40 + 16.5, 15), 16.5, new Frame());
			ShapeSPtr light = std::make_shared<Sphere>(Point3d(0, 640 - .5, 0), 600, new Frame());
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
			MaterialSPtr glass_mat = std::make_shared<Dielectric>(whiteConst, whiteConst, Fresnel::Glass);
			MaterialList materialList{ red, blue, gray, black,earth, mirror_mat, glass_mat, plastic_mat };

			std::shared_ptr<AreaLight> area_light = std::make_shared<AreaLight>(Color(10, 10, 10), light.get());
			LightList lightList{ area_light };


			PrimitiveList primitiveList
			{
				{   left.get(),   red.get(),nullptr },
				{  right.get(),  blue.get(),nullptr },
				{   back.get(),  gray.get(),nullptr },
				//{  front.get(), black.get(),nullptr },
				{ bottom.get(),  gray.get(),nullptr },
				{    top.get(),  gray.get(),nullptr },

				{ mirror.get(), mirror_mat.get(), nullptr },
				{  glass.get(),   glass_mat.get(), nullptr },

				{  light.get(), black.get(),area_light.get() }
			};

			return Scene{ shapeList, materialList, lightList, primitiveList ,textureList };
		}

		static Scene CreatHeadScene() {
			ImageSPtr texImg = imageManager.Add("diffuse.png");
			ImageSPtr earthImg = imageManager.Add("earthmap.png");
			ModelSPtr head = modelManager.Add("head.obj");

			auto shapeList = CreateTriangleMaeh(head, Frame({ 0,0,-1 }, { 0,1,0 }, { 1,0,0 }));

			ShapeSPtr light = std::make_shared<Sphere>(Point3d(0, 1e3 + 60, 0), 1e3, new Frame());
			ShapeSPtr sp = std::make_shared<Sphere>(Point3d(0, 0, 30), 10, new Frame());
			ShapeSPtr earth = std::make_shared<Sphere>(Point3d(0, -1e3 - 60, 0), 1e3, new Frame());
			ShapeSPtr disk = std::make_shared<Disk>(40, new Frame{ {1,0,0},{0,0,1},{0,-1,0},{0,50,0} });
			ShapeSPtr rect = std::make_shared<Rectangle>(50, 50, new Frame{ {1,0,0},{0,0,1},{0,-1,0},{0,-20,0} });
			//ShapeSPtr box = std::make_shared<Box>(Vector3d(5, 20, 20), new Frame{ {1,Sqrt2,1},{-1,Sqrt2,-1},{-1,0,1},{0,0,0} });
			ShapeSPtr box = std::make_shared<Box>(Vector3d(5, 10, 15), new Frame{ {1,0,0},{0,1,0},{0,0,1},{40,40,-40} });
			shapeList.insert(shapeList.end(), { light,sp ,earth,disk,rect,box });

			TextureSPtr blackConst = std::make_shared<ConstantTexture<Color>>(Color(.0, .0, .0));
			TextureSPtr redConst = std::make_shared<ConstantTexture<Color>>(Color(.75, .25, .25));
			std::shared_ptr<TextureFilter> bilinear = std::make_shared<BilinearFilter>();
			TextureSPtr textPtr =
				std::make_shared<ImageTexture<Color, Color>>(
					std::make_unique<UVMapping2D>(),
					bilinear, texImg);
			TextureSPtr earthPtr =
				std::make_shared<ImageTexture<Color, Color>>(
					std::make_unique<UVMapping2D>(),
					bilinear, earthImg);
			std::vector<TextureSPtr> textureList{ earthPtr, textPtr,blackConst ,redConst };

			MaterialSPtr headMat = std::make_shared<Matte>(textPtr);
			MaterialSPtr earthMat = std::make_shared<Matte>(earthPtr);
			MaterialSPtr black = std::make_shared<Matte>(blackConst);
			MaterialSPtr red = std::make_shared<Matte>(redConst);
			MaterialList materialList{ headMat ,black,red };

			std::shared_ptr<AreaLight> area_light = std::make_shared<AreaLight>(Color(1, 1, 1), light.get());
			std::shared_ptr<AreaLight> disk_light = std::make_shared<AreaLight>(Color(10, 10, 10), disk.get());
			LightList lightList{ area_light,disk_light };

			PrimitiveList primitiveList{
				//{ light.get(),red.get(),area_light.get()},
				//{sp.get(),headMat.get(),nullptr},
				//{rect.get(),red.get(),nullptr},
				{box.get(),red.get(),nullptr},
				{disk.get(),red.get(),disk_light.get()},
				{earth.get(),earthMat.get(),nullptr}
			};
			//for (int i = 0; i < head->nfaces(); i++) {
			//	primitiveList.push_back({ shapeList[i].get(),headMat.get(),nullptr });
			//}

			return Scene{ shapeList, materialList, lightList, primitiveList ,textureList };
		}

		static Scene CreatCornellBox() {
			double x, y, z;
			x = y = z = 35;

			ShapeSPtr disk = std::make_shared<Disk>(15, new Frame{ {1,0,0},{0,0,1},{0,-1,0},{0,y - .1,0} });
			ShapeSPtr up = std::make_shared<Rectangle>(Vector2d(x, y), new Frame{ {1,0,0},{0,0,1},{0,-1,0},{0,z,0 } });
			ShapeSPtr down = std::make_shared<Rectangle>(Vector2d(x, y), new Frame{ {1,0,0},{0,0,-1},{0,1,0},{0,-z,0 } });
			ShapeSPtr left = std::make_shared<Rectangle>(Vector2d(y, z), new Frame{ {0,0,-1},{0,1,0},{1,0,0},{-x,0,0} });
			ShapeSPtr right = std::make_shared<Rectangle>(Vector2d(y, z), new Frame{ {0,0,1},{0,1,0},{-1,0,0},{x,0,0} });
			ShapeSPtr back = std::make_shared<Rectangle>(Vector2d(x, z), new Frame{ {1,0,0},{0,1,0},{0,0,1},{0,0,-y} });
			ShapeSPtr box = std::make_shared<Box>(Vector3d(10, 10, 10), new Frame{ {4,0,-1},{0,1,0},{1,0,4},{x - 20,-y + 10,z - 20} });
			ShapeSPtr box2 = std::make_shared<Box>(Vector3d(10, 10, 20), new Frame{ {5,0,1},{0,1,0},{-1,0,5},{-x + 25,-y + 20,-z + 30} });
			ShapeSPtr cy = std::make_shared<Cylinder>(10, 40, new Frame{ {1,0,0},{0,0,-1},{0,1,0},{-x + 25,-y + 20,-z + 30} });
			ShapeSPtr cover = std::make_shared<Disk>(10, new Frame{ {1,0,0},{0,0,-1},{0,1,0},{-x + 25,-y + 40,-z + 30} });
			ShapeList shapeList{ disk,up,down,left,right,back,box,box2,cy,cover };


			TextureSPtr whiteConst = std::make_shared<ConstantTexture<Color>>(Color(1., 1., 1.));
			TextureSPtr redConst = std::make_shared<ConstantTexture<Color>>(Color(1., 0., 0.));
			TextureSPtr greenConst = std::make_shared<ConstantTexture<Color>>(Color(0., 1., 0.));
			TextureSPtr blackConst = std::make_shared<ConstantTexture<Color>>(Color(.0, .0, .0));
			TextureList textureList{ whiteConst ,redConst ,greenConst,blackConst };

			MaterialSPtr white = std::make_shared<Matte>(whiteConst);
			MaterialSPtr red = std::make_shared<Matte>(redConst);
			MaterialSPtr green = std::make_shared<Matte>(greenConst);
			MaterialSPtr black = std::make_shared<Matte>(blackConst);
			MaterialList materialList{ white,red,green,black };

			std::shared_ptr<AreaLight> disk_light = std::make_shared<AreaLight>(Color(5, 5, 5), disk.get());
			LightList lightList{ disk_light };

			PrimitiveList primitiveList{
				{box.get(),white.get(),nullptr},
				{cy.get(),red.get(),nullptr},
				{cover.get(),red.get(),nullptr},
				//{box2.get(),green.get(),nullptr},
				{up.get(),white.get(),nullptr},
				{down.get(),white.get(),nullptr},
				{back.get(),white.get(),nullptr},
				{left.get(),green.get(),nullptr},
				{right.get(),red.get(),nullptr},
				{disk.get(),white.get(),disk_light.get()}
			};

			return Scene{ shapeList, materialList, lightList, primitiveList ,textureList };
		}

	public:
		ShapeList shapeList;
		MaterialList materialList;
		LightList lightList;
		PrimitiveList primitiveList;
		TextureList textureList;
	};
}