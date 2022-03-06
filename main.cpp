#include "Scene.h"

#include <iostream>
#include <fstream>

#define CPP11

//	set max recursion depth
Vector3 rayColor(const Ray& ray, const Color& background, const HittableList& scene,int depth)
{
	HitRecord rec;

	//	If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
	{
		return Vector3(0.0, 0.0, 0.0);
	}

	if (!scene.hit(ray, 0.001, infinity, rec))
	{
		return background;
	}

	//	递归调用求多次反射，tMin = 0.001避免t=0时的光线自相交

	//Point3 target = rec.point + rec.normal + randomUnitVector(); // center + randomVector(x, y, z) x,y,z∈[-1,1]
	////	N的域是[-1,1]，需要变换到[0,1]
	////Point3 target = rec.point + randomInHemiSphere(rec.normal);
	//return 0.5 * rayColor(Ray(rec.point, target - rec.point), scene, depth - 1);

	Ray scattered;
	Color attenuation;
	Color emitted = rec.matptr->emitted(rec.u, rec.v, rec.point);

	if (rec.matptr->scatter(ray, rec, attenuation, scattered))
	{
		return emitted + attenuation * rayColor(scattered, background, scene, depth - 1);
	}
	return emitted;
}

int main()
{
	//	Image
	double aspectRatio = 16.0 / 9.0;
	int imageWidth = 400;
	int imageHeight = static_cast<int>(imageWidth / aspectRatio);
	
	int samplesPerPixel = 100;
	int maxDepth = 50;

	//	Scene
	HittableList scene;
	Color background(0.0, 0.0, 0.0);

	//	Camera
	Point3 camPos;
	Point3 target;
	Vector3 up(0.0, 1.0, 0.0);
	double focus = 10.0;
	double aperture = 0.0;
	double fov = 20.0;

	switch (8)
	{
	case 1:
		scene = oneWeekendRandomScene();
		background = Color(0.70, 0.80, 1.00);
		break;

	case 2:
		scene = twoCheckerMapSpheres();
		background = Color(0.70, 0.80, 1.00);
		break;

	case 3:
		scene = twoPerlinSpheres();
		background = Color(0.70, 0.80, 1.00);
		break;

	case 4:
		scene = earth();
		background = Color(0.70, 0.80, 1.00);
		break;

	case 5:
		scene = simpleLight();
		camPos = Point3(26, 3, 6);
		target = Point3(0, 2, 0);
		background = Color(0.0, 0.0, 0.0);
		samplesPerPixel = 400;
		fov = 20.0;
		break;

	case 6:
		scene = cornellBox();
		aspectRatio = 1.0;
		imageWidth = 600;
		imageHeight = static_cast<int>(imageWidth / aspectRatio);
		samplesPerPixel = 200;
		background = Color(0.0, 0.0, 0.0);
		camPos = Point3(278, 278, -800);
		target = Point3(278, 278, 0);
		fov = 40.0;
		break;

	case 7:
		scene = cornellSmoke();
		aspectRatio = 1.0;
		imageWidth = 600;
		imageHeight = static_cast<int>(imageWidth / aspectRatio);
		samplesPerPixel = 200;
		background = Color(0.0, 0.0, 0.0);
		camPos = Point3(278, 278, -800);
		target = Point3(278, 278, 0);
		fov = 40.0;
		break;

	case 8:
		scene = finalScene();
		aspectRatio = 1.0;
		imageWidth = 800;
		imageHeight = static_cast<int>(imageWidth / aspectRatio);
		samplesPerPixel = 100;
		background = Color(0.0, 0.0, 0.0);
		camPos = Point3(478, 278, -600);
		target = Point3(278, 278, 0);
		fov = 40.0;
		break;

	default:
		break;
	}

	Camera cam(camPos, target, up, fov, aspectRatio, aperture, focus, 0.0, 1.0);

	//	Render

	std::ofstream out;
	out.open("./result/NW/theNextWeek.ppm");

	out << "P3\n" << imageWidth << ' ' << imageHeight << "\n256\n";

	for (int i = imageHeight - 1; i >= 0; --i)
	{
		std::cerr << "\rScanlines remaining: " << i << ' ' << std::flush;
		for (int j = 0; j < imageWidth; ++j)
		{
			//	MSAA
			Color pixelColor(0.0, 0.0, 0.0);
			for (int s = 0; s < samplesPerPixel; ++s)
			{
				double u = (j + C11randomDouble()) / (imageWidth - 1);
				double v = (i + C11randomDouble()) / (imageHeight - 1);
				Ray ray = cam.getRay(u, v);
				pixelColor += rayColor(ray, background, scene, maxDepth);
			}
			writeColor(out, pixelColor, samplesPerPixel);
		}
	}
	std::cerr << "\nDone.\n";
}