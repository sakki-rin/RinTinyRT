#pragma once
#ifndef _SCENE_H
#define _SCENE_H

#include "HittableList.h"
#include "Camera.h"
#include "Material.h"
#include "Color.h"
#include "Utilities.h"
#include "Sphere.h"
#include "BVH.h"
#include "Rect.h"
#include "Box.h"
#include "ConstMedium.h"

HittableList oneWeekendRandomScene()
{
	HittableList scene;

	Material::Ptr groundMaterial = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
	scene.add(make_shared<Sphere>(Point3(0.0, -1000.0, 0.0), 1000.0, groundMaterial));

	for (int a = -11; a < 11; ++a)
	{
		for (int b = -11; b < 11; ++b)
		{
			double chooseMaterial = randomDouble();
			Point3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

			if ((center - Point3(4.0, 0.2, 0.0)).length() > 0.9)
			{
				Material::Ptr sphereMaterial;

				if (chooseMaterial < 0.8)
				{
					//	diffuse
					Vector3 albedo = Color::random() * Color::random();
					sphereMaterial = make_shared<Lambertian>(albedo);
					scene.add(make_shared<MovingSphere>(center, center + Vector3(0.0, randomDouble(0, 0.5), 0.0), 0.2, 0.0, 1.0, sphereMaterial));
				}
				else if (chooseMaterial < 0.95)
				{
					//	Metal
					Vector3 albedo = Color::random(0.5, 1.0);
					double fuzz = randomDouble(0.0, 0.5);
					sphereMaterial = make_shared<Metal>(albedo, fuzz);
					scene.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
				else
				{
					//	glass
					sphereMaterial = make_shared<Dielectric>(1.5);
					scene.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
			}
		}
	}
	auto material1 = make_shared<Dielectric>(1.5);
	scene.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
	scene.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.2);
	scene.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

	return scene;
}

//	4.3
HittableList CheckerTextureRandomScene()
{
	HittableList scene;

	Texture::Ptr checker = make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
	Material::Ptr groundMaterial = make_shared<Lambertian>(checker);
	scene.add(make_shared<Sphere>(Point3(0.0, -1000.0, 0.0), 1000.0, groundMaterial));

	for (int a = -11; a < 11; ++a)
	{
		for (int b = -11; b < 11; ++b)
		{
			double chooseMaterial = randomDouble();
			Point3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

			if ((center - Point3(4.0, 0.2, 0.0)).length() > 0.9)
			{
				Material::Ptr sphereMaterial;

				if (chooseMaterial < 0.8)
				{
					//	diffuse
					Vector3 albedo = Color::random() * Color::random();
					sphereMaterial = make_shared<Lambertian>(albedo);
					scene.add(make_shared<MovingSphere>(center, center + Vector3(0.0, randomDouble(0, 0.5), 0.0), 0.2, 0.0, 1.0, sphereMaterial));
				}
				else if (chooseMaterial < 0.95)
				{
					//	Metal
					Vector3 albedo = Color::random(0.5, 1.0);
					double fuzz = randomDouble(0.0, 0.5);
					sphereMaterial = make_shared<Metal>(albedo, fuzz);
					scene.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
				else
				{
					//	glass
					sphereMaterial = make_shared<Dielectric>(1.5);
					scene.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
			}
		}
	}
	auto material1 = make_shared<Dielectric>(1.5);
	scene.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
	scene.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.1);
	scene.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

	return HittableList(make_shared<BVHNode>(scene, 0.0, 1.0));;
}

//	4.4
HittableList twoCheckerMapSpheres()
{
	HittableList scene;

	auto checker = make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
	auto material1 = make_shared<Lambertian>(checker);

	scene.add(make_shared<Sphere>(Point3(0, -10, 0), 10, material1));
	scene.add(make_shared<Sphere>(Point3(0, 10, 0), 10, material1));

	return scene;
}

#endif // !_SCENE_H

//	5.7 perlin noise
HittableList twoPerlinSpheres()
{
	HittableList scene;

	//	µÕ∆µ‘Î…˘
	//auto pertext = make_shared<NoiseTexture>();
	//	∏ﬂ∆µ‘Î…˘
	auto pertext = make_shared<NoiseTexture>(4);
	auto perMaterial = make_shared<Lambertian>(pertext);

	scene.add(make_shared<Sphere>(Vector3(0.0, -1000.0, 0.0), 1000.0, perMaterial));
	scene.add(make_shared<Sphere>(Vector3(0.0, 2.0, 0.0), 2.0, perMaterial));

	return scene;
}

//	6 Image Texture
HittableList earth()
{
	HittableList scene;

	auto earthTex = make_shared<ImageTexture>("earthmap.jpg");
	auto earthMat = make_shared<Lambertian>(earthTex);

	auto earth = make_shared<Sphere>(Point3(0.0, 0.0, 0.0), 2.0, earthMat);
	scene.add(earth);
	
	return scene;
}

//	7 Rectanles and lights
HittableList simpleLight()
{
	HittableList scene;

	auto perTex = make_shared<NoiseTexture>(4);
	auto perMaterial = make_shared<Lambertian>(perTex);

	scene.add(make_shared<Sphere>(Point3(0.0, -1000.0, 0.0), 1000.0, perMaterial));
	scene.add(make_shared<Sphere>(Point3(0.0, 2.0, 0.0), 2.0, perMaterial));

	auto diffLight = make_shared<DiffuseLight>(Color(4, 4, 4));
	scene.add(make_shared<Sphere>(Point3(0, 7, 0), 2, diffLight));
	scene.add(make_shared<XYRect>(3, 5, 1, 3, -2, diffLight));

	return scene;
}

//	7.6 Cornell Box
HittableList cornellBox()
{
	HittableList scene;

	auto red = make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
	auto white = make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
	auto green = make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
	auto light = make_shared<DiffuseLight>(Color(15, 15, 15));

	scene.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
	scene.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
	scene.add(make_shared<XZRect>(213, 343, 227, 332, 554, light));
	scene.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
	scene.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
	scene.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));

	Hittable::Ptr box1 = make_shared<Box>(Point3(0.0, 0.0, 0.0), Point3(165, 330, 165), white);
	box1 = make_shared<RotateY>(box1, 15);
	box1 = make_shared<Translate>(box1, Vector3(265, 0, 295));

	Hittable::Ptr box2 = make_shared<Box>(Point3(0.0, 0.0, 0.0), Point3(165, 165, 165), white);
	box2 = make_shared<RotateY>(box2, -18);
	box2 = make_shared<Translate>(box2, Vector3(130, 0, 65));

	scene.add(box1);
	scene.add(box2);

	return scene;
}

//	9.Volumes
HittableList cornellSmoke()
{
	HittableList scene;

	auto red = make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
	auto white = make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
	auto green = make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
	auto light = make_shared<DiffuseLight>(Color(7, 7, 7));

	scene.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
	scene.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
	scene.add(make_shared<XZRect>(113, 443, 127, 432, 554, light));
	scene.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
	scene.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
	scene.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));

	Hittable::Ptr box1 = make_shared<Box>(Point3(0.0, 0.0, 0.0), Point3(165, 330, 165), white);
	box1 = make_shared<RotateY>(box1, 15);
	box1 = make_shared<Translate>(box1, Vector3(265, 0, 295));

	Hittable::Ptr box2 = make_shared<Box>(Point3(0.0, 0.0, 0.0), Point3(165, 165, 165), white);
	box2 = make_shared<RotateY>(box2, -18);
	box2 = make_shared<Translate>(box2, Vector3(130, 0, 65));

	box1 = make_shared<ConstMedium>(box1, 0.01, Color(0, 0, 0));
	box2 = make_shared<ConstMedium>(box2, 0.01, Color(1, 1, 1));

	scene.add(box1);
	scene.add(box2);

	return scene;
}

//	the next week
HittableList finalScene()
{
	HittableList boxes1;
	auto ground = make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

	const int boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; i++) {
		for (int j = 0; j < boxes_per_side; j++) {
			auto w = 100.0;
			auto x0 = -1000.0 + i * w;
			auto z0 = -1000.0 + j * w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto y1 = randomDouble(1, 101);
			auto z1 = z0 + w;

			boxes1.add(make_shared<Box>(Point3(x0, y0, z0), Point3(x1, y1, z1), ground));
		}
	}

	HittableList objects;

	objects.add(make_shared<BVHNode>(boxes1, 0, 1));

	auto light = make_shared<DiffuseLight>(Color(7, 7, 7));
	objects.add(make_shared<XZRect>(123, 423, 147, 412, 554, light));

	auto center1 = Point3(400, 400, 200);
	auto center2 = center1 + Vector3(30, 0, 0);
	auto moving_sphere_material = make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
	objects.add(make_shared<MovingSphere>(center1, center2, 0, 1, 50, moving_sphere_material));

	objects.add(make_shared<Sphere>(Point3(260, 150, 45), 50, make_shared<Dielectric>(1.5)));
	objects.add(make_shared<Sphere>(
		Point3(0, 150, 145), 50, make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)
		));

	auto boundary = make_shared<Sphere>(Point3(360, 150, 145), 70, make_shared<Dielectric>(1.5));
	objects.add(boundary);
	objects.add(make_shared<ConstMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
	boundary = make_shared<Sphere>(Point3(0, 0, 0), 5000, make_shared<Dielectric>(1.5));
	objects.add(make_shared<ConstMedium>(boundary, .0001, Color(1, 1, 1)));

	auto emat = make_shared<Lambertian>(make_shared<ImageTexture>("earthmap.jpg"));
	objects.add(make_shared<Sphere>(Point3(400, 200, 400), 100, emat));
	auto pertext = make_shared<NoiseTexture>(0.1);
	objects.add(make_shared<Sphere>(Point3(220, 280, 300), 80, make_shared<Lambertian>(pertext)));

	HittableList boxes2;
	auto white = make_shared<Lambertian>(Color(.73, .73, .73));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxes2.add(make_shared<Sphere>(Point3::random(0, 165), 10, white));
	}

	objects.add(make_shared<Translate>(
		make_shared<RotateY>(
			make_shared<BVHNode>(boxes2, 0.0, 1.0), 15),
		Vector3(-100, 270, 395)
		)
	);

	return objects;
}