#pragma once
#ifndef _CONSTMEDIUM_H
#define _CONSTMEDIUM_H

#include "Hittable.h"
#include "Utilities.h"
#include "Material.h"

class ConstMedium : public Hittable
{
public:

	ConstMedium(Hittable::Ptr b, double d, Texture::Ptr tp)
		: boundary(b), negInvDensity(-1 / d), phaseFunc(make_shared<Iostropic>(tp))
	{}

	ConstMedium(Hittable::Ptr b,double d, Color c)
		: boundary(b),negInvDensity(-1/d),phaseFunc(make_shared<Iostropic>(c))
	{}

	virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRec) const override;

	virtual bool boundingBox(double t0, double t1, AABB& outputBox) const override
	{
		return boundary->boundingBox(t0, t1, outputBox);
	}

public:

	Hittable::Ptr boundary;

	Material::Ptr phaseFunc;

	double negInvDensity;
};

bool ConstMedium::hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRec) const
{
	const bool enableDebug = false;
	const bool debugging = enableDebug && randomDouble() < 0.00001;

	HitRecord rec1, rec2;

	if (!boundary->hit(ray, -infinity, infinity, rec1))
	{
		return false;
	}

	if (!boundary->hit(ray, rec1.t + 0.0001, infinity, rec2))
	{
		return false;
	}

	if (debugging)
	{
		std::cerr << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << '\n';
	}

	if (rec1.t < tMin)
	{
		rec1.t = tMin;
	}
	if (rec2.t > tMax)
	{
		rec2.t = tMax;
	}

	if (rec1.t < 0)
	{
		rec1.t = 0;
	}

	const auto rayLength = ray.direction().length();
	const auto distanceInsideBoundary = (rec2.t - rec1.t) * rayLength;
	const auto hitDistance = negInvDensity * log(randomDouble());

	if (hitDistance > distanceInsideBoundary)
	{
		return false;
	}

	hitRec.t = rec1.t + hitDistance / rayLength;
	hitRec.point = ray.at(hitRec.t);

	if (debugging)
	{
		std::cerr << "hit_distance = " << hitDistance << '\n'
			<< "rec.t = " << hitRec.t << '\n'
			<< "rec.p = " << hitRec.point << '\n';
	}

	hitRec.normal = Vector3(0.0, 0.0, 0.0);
	hitRec.frontFace = true;
	hitRec.matptr = phaseFunc;

	return true;
}

#endif // !_CONSTMEDIUM_H
