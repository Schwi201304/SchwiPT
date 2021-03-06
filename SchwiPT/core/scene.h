#pragma once

#include<core/schwi.h>
#include<material/material.h>
#include<material/skin.h>
#include<core/primitive.h>
#include<shapes/shape.h>
#include<shapes/sphere.h>
#include<shapes/triangle.h>
#include<shapes/disk.h>
#include<shapes/rectangle.h>
#include<shapes/cylinder.h>
#include<textures/imagemap.h>
#include<textures/constant.h>
#include<textures/cubemap.h>

namespace schwi {
	class Scene {
	public:
		ShapeList shapeList;
		MaterialList materialList;
		LightList lightList;
		PrimitiveList primitiveList;
		TextureList textureList;
		CubeMap* skybox;

	public:
		Scene() = default;
		Scene(ShapeList shapeList, MaterialList materialList,
			LightList lightList, PrimitiveList primitiveList,
			TextureList textureList, CubeMap* skybox=nullptr) :
			shapeList(shapeList),
			materialList(materialList),
			lightList(lightList),
			primitiveList(primitiveList),
			textureList(textureList),
			skybox(skybox) {}

	public:
		bool Intersect(Ray& ray, SurfaceIntersection* isect)const {
			bool hit = false;

			for (const auto& primitive : primitiveList) {
				if (primitive.Intersect(ray, isect)) {
					hit = true;
				}
			}

			return hit;
		}

		bool Occluded(Point3d& position, const Vector3d& direction, double distance) {
			Ray ray(position + direction * MachineEpsilon, direction, distance - Epsilon);
			return Intersect(ray, new SurfaceIntersection());
		}

		bool Occluded(SurfaceIntersection& isect1, const Point3d& isect2) {
			Ray ray = isect1.GenerateRay(isect2);
			isect1.NoL = Dot(isect1.normal, (isect2 - isect1.position).Normalize());
			ray.set_distance(Distance(isect1.position, isect2) - 2 * 0.1);
			return Intersect(ray, new SurfaceIntersection());
		}

		bool Occluded(SurfaceIntersection& isect1, const SurfaceIntersection& isect2) {
			Ray ray = isect1.GenerateRay(isect2);
			isect1.NoL = Dot(isect1.normal, (isect2.position - isect1.position).Normalize());
			ray.set_distance(Distance(isect1.position, isect2.position) - 2 * Epsilon);
			return Intersect(ray, new SurfaceIntersection());
		}

		Bounds3d WorldBound()const {
			Bounds3d bound;
			for (const auto& primitive : primitiveList) {
				bound = Union(bound, primitive.shape->WorldBound());
			}
			return bound;
		}

		Color SkyBox(const Vector3d& v) {
			return skybox == nullptr ? Color() :
				skybox->Evaluate(v);
		}

	public:
		/*
		* ????????????????????????
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

			std::shared_ptr<AreaLight> area_light = std::make_shared<AreaLight>(light->frame->position(), 1, Color(10, 10, 10), light.get());
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
		*/
		static Scene CreatHeadScene() {
			ImageSPtr texImg = imageManager.Add("diffuse.png");
			ImageSPtr earthImg = imageManager.Add("earthmap.png");
			ModelSPtr head = modelManager.Add("head.obj");

			auto shapeList = CreateTriangleMaeh(head, Frame({ 0,0,-1 }, { 0,1,0 }, { 1,0,0 }));

			ShapeSPtr light = std::make_shared<Sphere>(1e3, new Frame({ 0, 0, -1 }, { 0,1,0 }, { 1,0,0 }, { 0, 1e3 + 60, 0 }));
			ShapeSPtr sp = std::make_shared<Sphere>(10, new Frame({ 0,0,-1 }, { 0,1,0 }, { 1,0,0 }, { 0, 0, 30 }));
			ShapeSPtr earth = std::make_shared<Sphere>(1e3, new Frame({ 0,0,-1 }, { 0,1,0 }, { 1,0,0 }, { 0, -1e3 - 60, 0 }));
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

			std::shared_ptr<AreaLight> area_light = std::make_shared<AreaLight>(light->frame->position(), 1, Color(1, 1, 1), light.get());
			std::shared_ptr<AreaLight> disk_light = std::make_shared<AreaLight>(disk->frame->position(), 1, Color(10, 10, 10), disk.get());
			LightList lightList{ area_light,disk_light };

			PrimitiveList primitiveList{
				//{ light.get(),red.get(),area_light.get()},
				//{sp.get(),headMat.get(),nullptr},
				//{rect.get(),red.get(),nullptr},
				//{box.get(),red.get(),nullptr},
				{disk.get(),red.get(),disk_light.get()},
				{earth.get(),earthMat.get(),nullptr}
			};
			for (int i = 0; i < head->nfaces(); i++) {
				primitiveList.push_back({ shapeList[i].get(),headMat.get(),nullptr });
			}

			return Scene{ shapeList, materialList, lightList, primitiveList ,textureList };
		}

		static Scene CreatCornellBox() {
			double x, y, z;
			x = y = z = 35;
			ImageSPtr earthImg = imageManager.Add("earthmap.png");

			ShapeSPtr disk = std::make_shared<Disk>(15, new Frame{ {1,0,0},{0,0,1},{0,-1,0},{0,y - .1,0} });
			ShapeSPtr up = std::make_shared<Rectangle>(Vector2d(x, y), new Frame{ {1,0,0},{0,0,1},{0,-1,0},{0,z,0 } });
			ShapeSPtr down = std::make_shared<Rectangle>(Vector2d(x, y), new Frame{ {1,0,0},{0,0,-1},{0,1,0},{0,-z,0 } });
			ShapeSPtr left = std::make_shared<Rectangle>(Vector2d(y, z), new Frame{ {0,0,-1},{0,1,0},{1,0,0},{-x,0,0} });
			ShapeSPtr right = std::make_shared<Rectangle>(Vector2d(y, z), new Frame{ {0,0,1},{0,1,0},{-1,0,0},{x,0,0} });
			ShapeSPtr back = std::make_shared<Rectangle>(Vector2d(x, z), new Frame{ {1,0,0},{0,1,0},{0,0,1},{0,0,-y} });
			ShapeSPtr box = std::make_shared<Box>(Vector3d(10, 10, 10), new Frame{ {4,0,-1},{0,1,0},{1,0,4},{x - 20,-y + 10,z - 20} });
			ShapeSPtr box2 = std::make_shared<Box>(Vector3d(10, 10, 20), new Frame{ {5,0,1},{0,1,0},{-1,0,5},{-x + 25,-y + 20,-z + 30} });
			ShapeSPtr cy = std::make_shared<Cylinder>(10, 40, new Frame{ {0,0,1},{1,0,0},{0,1,0},{-x + 25,-y + 20,-z + 30} });
			ShapeSPtr cover = std::make_shared<Disk>(10, new Frame{ {0,0,1},{1,0,0},{0,1,0},{-x + 25,-y + 40,-z + 30} });
			ShapeSPtr sphere = std::make_shared<Sphere>(10, new Frame{ {0,0,1},{1,0,0},{0,1,0},{x - 20,-y + 30,z - 20} });
			ShapeList shapeList{ disk,up,down,left,right,back,box,box2,cy,cover,sphere };

			std::shared_ptr<TextureFilter> bilinear = std::make_shared<BilinearFilter>();
			TextureSPtr earthPtr =
				std::make_shared<ImageTexture<Color, Color>>(
					std::make_unique<UVMapping2D>(),
					bilinear, earthImg);
			
			TextureSPtr whiteConst = std::make_shared<ConstantTexture<Color>>(Color(1., 1., 1.));
			TextureSPtr grayConst = std::make_shared<ConstantTexture<Color>>(Color(.75, .75, .75));
			TextureSPtr redConst = std::make_shared<ConstantTexture<Color>>(Color(1., 0., 0.));
			TextureSPtr greenConst = std::make_shared<ConstantTexture<Color>>(Color(0., 1., 0.));
			TextureSPtr blueConst = std::make_shared<ConstantTexture<Color>>(Color(0., 0., 1.));
			TextureSPtr blackConst = std::make_shared<ConstantTexture<Color>>(Color(.0, .0, .0));
			TextureSPtr skinConst = std::make_shared<ConstantTexture<Color>>(Color(119. / 255., 84. / 255., 77. / 255.));
			TextureList textureList{ whiteConst ,grayConst,redConst ,greenConst,blueConst,blackConst,earthPtr ,skinConst };

			MaterialSPtr white = std::make_shared<Matte>(whiteConst);
			MaterialSPtr red = std::make_shared<Matte>(redConst);
			MaterialSPtr green = std::make_shared<Matte>(greenConst);
			MaterialSPtr blue = std::make_shared<Matte>(blueConst);
			MaterialSPtr black = std::make_shared<Matte>(blackConst);
			MaterialSPtr mirror = std::make_shared<Mirror>(whiteConst,0.5);
			MaterialSPtr glass = std::make_shared<Glass>(whiteConst, Fresnel::Glass);
			MaterialSPtr earthMat = std::make_shared<Matte>(earthPtr);
			MaterialSPtr plastic = std::make_shared<Plastic>(whiteConst, whiteConst, 1);
			MaterialSPtr skin = std::make_shared<PreIntegraterdSkin>(skinConst);
			MaterialSPtr stock = std::make_shared<Stockings>(blackConst, whiteConst, 50);
			MaterialList materialList{ white,red,green,blue,black,mirror,glass,earthMat,plastic,skin,stock };

			std::shared_ptr<AreaLight> disk_light = std::make_shared<AreaLight>(disk->frame->position(), 1, Color(5, 5, 5), disk.get());
			std::shared_ptr<Light> point_light = std::make_shared<PointLight>(disk->frame->position(), 1, Color(5, 5, 5));
			LightList lightList{ disk_light };

			PrimitiveList primitiveList{
				{box.get(),white.get(),nullptr},
				{cy.get(),mirror.get(),nullptr},
				{cover.get(),white.get(),nullptr},
				{sphere.get(),glass.get(),nullptr},
				//{box2.get(),white.get(),nullptr},
				{up.get(),white.get(),nullptr},
				{down.get(),white.get(),nullptr},
				{back.get(),mirror.get(),nullptr},
				{left.get(),green.get(),nullptr},
				{right.get(),red.get(),nullptr},
				{disk.get(),white.get(),disk_light.get()}
			};

			ImageSPtr cubeMap = imageManager.Add("sky1.png");
			CubeMap* skybox = new CubeMap(cubeMap);

			return Scene{ shapeList, materialList, lightList, primitiveList ,textureList,skybox };
		}

		static Scene LightSample() {
			double x = 40, y = 12;
			ShapeSPtr rect1 = std::make_shared<Rectangle>(Vector2d(x, y), new Frame{ {1,0,0},{0,1,-4},{0,4,1},{0,-20,30 } });
			ShapeSPtr rect2 = std::make_shared<Rectangle>(Vector2d(x, y), new Frame{ {1,0,0},{0,1,-3},{0,3,1},{0,-12,0 } });
			ShapeSPtr rect3 = std::make_shared<Rectangle>(Vector2d(x, y), new Frame{ {1,0,0},{0,1,-2},{0,2,1},{0,-2,-30 } });
			ShapeSPtr rect4 = std::make_shared<Rectangle>(Vector2d(x, y), new Frame{ {1,0,0},{0,1,-1},{0,1,1},{0,15,-60 } });
			ShapeSPtr sp1 = std::make_shared<Sphere>(8, new Frame{ {0,0,1},{1,0,0},{0,1,0},{25,15,5} });
			ShapeSPtr sp2 = std::make_shared<Sphere>(6, new Frame{ {0,0,1},{1,0,0},{0,1,0},{5,15,5} });
			ShapeSPtr sp3 = std::make_shared<Sphere>(4, new Frame{ {0,0,1},{1,0,0},{0,1,0},{-12,15,5} });
			ShapeSPtr sp4 = std::make_shared<Sphere>(2, new Frame{ {0,0,1},{1,0,0},{0,1,0},{-30,15,5} });
			ShapeList shapeList{ rect1,rect2,rect3,rect4,sp1,sp2,sp3,sp4 };

			TextureSPtr whiteConst = std::make_shared<ConstantTexture<Color>>(Color(1., 1., 1.));
			TextureSPtr grayConst = std::make_shared<ConstantTexture<Color>>(Color(.75, .75, .75));
			TextureSPtr redConst = std::make_shared<ConstantTexture<Color>>(Color(1., 0., 0.));
			TextureSPtr greenConst = std::make_shared<ConstantTexture<Color>>(Color(0., 1., 0.));
			TextureSPtr blueConst = std::make_shared<ConstantTexture<Color>>(Color(0., 0., 1.));
			TextureSPtr yellowConst = std::make_shared<ConstantTexture<Color>>(Color(1., 1., 0.));
			TextureSPtr blackConst = std::make_shared<ConstantTexture<Color>>(Color(.0, .0, .0));
			TextureList textureList{ whiteConst,grayConst ,redConst ,greenConst,blueConst,yellowConst,blackConst };

			MaterialSPtr white = std::make_shared<Matte>(whiteConst);
			MaterialSPtr gray = std::make_shared<Matte>(grayConst);
			MaterialSPtr red = std::make_shared<Matte>(redConst);
			MaterialSPtr green = std::make_shared<Matte>(greenConst);
			MaterialSPtr blue = std::make_shared<Matte>(blueConst);
			MaterialSPtr yellow = std::make_shared<Matte>(yellowConst);
			MaterialSPtr black = std::make_shared<Matte>(blackConst);
			MaterialSPtr mirror = std::make_shared<Mirror>(whiteConst,0.5);
			MaterialSPtr plastic = std::make_shared<Plastic>(whiteConst, yellowConst, 1);
			MaterialList materialList{ white,gray,red,green,blue,yellow,black ,plastic,mirror };

			std::shared_ptr<AreaLight> sp1_light = std::make_shared<AreaLight>(sp1->frame->position(), 1, Color(0., 0., 5.), sp1.get());
			std::shared_ptr<AreaLight> sp2_light = std::make_shared<AreaLight>(sp2->frame->position(), 1, Color(5., 5., 0.), sp2.get());
			std::shared_ptr<AreaLight> sp3_light = std::make_shared<AreaLight>(sp3->frame->position(), 1, Color(0., 5., 0.), sp3.get());
			std::shared_ptr<AreaLight> sp4_light = std::make_shared<AreaLight>(sp4->frame->position(), 1, Color(5., 0., 0.), sp4.get());
			LightList lightList{ sp1_light,sp2_light,sp3_light,sp4_light };

			PrimitiveList primitiveList{
				{rect1.get(),red.get(),nullptr },
				{rect2.get(),red.get(),nullptr },
				{rect3.get(),red.get(),nullptr },
				{rect4.get(),red.get(),nullptr },
				{sp1.get(),white.get(),sp1_light.get()},
				{sp2.get(),white.get(),sp2_light.get()},
				{sp3.get(),white.get(),sp3_light.get()},
				{sp4.get(),white.get(),sp4_light.get()}
			};

			return Scene{ shapeList, materialList, lightList, primitiveList ,textureList };
		}

		static Scene SkinTestScene() {
			ImageSPtr SkinLUT = imageManager.Add("SkinLUT.png");

			ShapeSPtr disk = std::make_shared<Disk>(30, new Frame{ {1,1,0},{0,0,1},{1,-1,0},{-60,60,0} });
			ShapeSPtr sphere = std::make_shared<Sphere>(30, new Frame({ 1,0,0 }, { 0,1,0 }, { 0,0,1 }, { 0,0,0 }));
			ShapeList shapeList{ disk,sphere };

			std::shared_ptr<TextureFilter> bilinear = std::make_shared<BilinearFilter>();
			TextureSPtr skinTex =
				std::make_shared<ImageTexture<Color, Color>>(
					std::make_unique<SkinMapping2D>(true, .25),
					bilinear, SkinLUT);
			TextureSPtr whiteConst = std::make_shared<ConstantTexture<Color>>(Color(1., 1., 1.));
			TextureList textureList{ whiteConst };

			MaterialSPtr white = std::make_shared<Matte>(whiteConst);
			MaterialSPtr skin = std::make_shared<PreIntegraterdSkin>(skinTex);
			MaterialList materialList{ white,skin };

			std::shared_ptr<AreaLight> disk_light = std::make_shared<AreaLight>(disk->frame->position(), 1, Color(30, 30, 30), disk.get());
			LightList lightList{ disk_light };

			PrimitiveList primitiveList{
				{sphere.get(),skin.get(),nullptr},
				{disk.get(),white.get(),disk_light.get()}
			};

			return Scene{ shapeList, materialList, lightList, primitiveList ,textureList };
		}
	};
}