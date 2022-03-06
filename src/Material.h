#pragma once
#ifndef _METERIAL_H
#define _METERIAL_H

#include "Ray.h"
#include "Utilities.h"
#include "Vector3.h"
#include "Hittable.h"
#include "Texture.h"

class Material
{
public:

	using Ptr = shared_ptr<Material>;

	virtual bool scatter(const Ray& RayIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const = 0;

	virtual Color emitted(double u, double v, const Point3& p) const
	{
		return Color(0.0, 0.0, 0.0);
	}
};

class Lambertian : public Material
{
public:

	Lambertian(const Color& a) : albedo(make_shared<ConstTexture>(a)) {}

	Lambertian(Texture::Ptr a) : albedo(a) {}

	virtual bool scatter(
		const Ray& RayIn, const HitRecord& rec,
		Vector3& attenuation, Ray& scattered
	) const override
	{
		//	lambda
		//auto scatterDir = [=]()->Vector3 {
		//	Vector3 dir;
		//	dir = rec.normal + randomUnitVector();

		//	//	避免零散射向量
		//	auto isZero = [dir]()->bool {return (fabs(dir.x()) < 1e-8) && (fabs(dir.y()) < 1e-8) && (fabs(dir.z()) < 1e-8); };

		//	if (isZero())
		//	{
		//		dir = rec.normal;
		//	}
		//};

		Vector3 scatterDir = rec.normal + randomUnitVector();

		//	Catch degenerate scatter direction
		if (scatterDir.nearZero())
		{
			scatterDir = rec.normal;
		}

		scattered = Ray(rec.point, scatterDir, RayIn.time());	//	默认为与入射光线时刻相同
		attenuation = albedo->texValue(rec.u, rec.v, rec.point);
		return true;
	}

public:

	Texture::Ptr albedo;
};

class Metal : public Material
{
public:
	
	Metal(const Color& a, double f) : albedo(a), fuzz(f) {}

	virtual bool scatter(
		const Ray& RayIn, const HitRecord& rec,
		Vector3& attenuation, Ray& scattered
	) const override
	{
		Vector3 reflected = reflect(normalize(RayIn.direction()), rec.normal);
		scattered = Ray(rec.point, reflected + fuzz * randomVecInUnitSphere(), RayIn.time());
		//scattered = Ray(rec.point, reflected + fuzz * randomUnitVector(), RayIn.time());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0.0);
	}

public:

	Color albedo; // 反射率
	double fuzz; //	模糊反射
};

class Dielectric : public Material
{
public:

	Dielectric(double _refraction) : refraction(_refraction) {}

	virtual bool scatter(
		const Ray& RayIn, const HitRecord& rec,
		Vector3& attenuation, Ray& scattered
	) const override
	{
		attenuation = Color(1.0, 1.0, 1.0);
		double refractionRatio = rec.frontFace ? (1.0 / refraction) : refraction;

		Vector3 rayInDir = normalize(RayIn.direction());
		double cosTheta = ffmin(dot(-rayInDir, rec.normal), 1.0);
		double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

		double sinThetaPrime = refractionRatio * sinTheta;	//	折射角sin

		Vector3 direction;

		//	发生折射的概率会随着入射角而改变
		if (sinThetaPrime > 1.0||schlick(cosTheta, refractionRatio)>randomDouble())
		{
			//	cannot refract, only relect happened
			direction = reflect(rayInDir, rec.normal);
		}
		else
		{
			//	refract happened
			direction = refract(rayInDir, rec.normal, refractionRatio);
		}

		scattered = Ray(rec.point, direction, RayIn.time());
		return true;
	}

public:

	double refraction;	// index of refraction

private:

	static double schlick(double cosine, double refIdx)
	{
		//	Use Schlick's approximation for reflectance
		double r0 = (1 - refIdx) / (1 + refIdx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};

class DiffuseLight : public Material
{
public:

	DiffuseLight(Texture::Ptr tex) : emit(tex) {}

	DiffuseLight(Color color) : emit(make_shared<ConstTexture>(color)) {}

	//	光源材质不进行散射
	virtual bool scatter(
		const Ray& RayIn, const HitRecord& rec,
		Vector3& attenuation, Ray& scattered
	) const override
	{
		return false;
	}

	virtual Color emitted(double u, double v, const Point3& p) const override
	{
		return emit->texValue(u, v, p);
	}

public:

	Texture::Ptr emit;
};

class Iostropic : public Material
{
public:

	Iostropic(Color c) : albedo(make_shared<ConstTexture>(c)) {}

	Iostropic(Texture::Ptr a) : albedo(a) {}

	virtual bool scatter(
		const Ray& RayIn, const HitRecord& rec,
		Vector3& attenuation, Ray& scattered
	) const override
	{
		scattered = Ray(rec.point, randomVecInUnitSphere(), RayIn.time());
		attenuation = albedo->texValue(rec.u, rec.v, rec.point);
		return true;
	}

public:

	Texture::Ptr albedo;
};

#endif // !_METERIAL_H
