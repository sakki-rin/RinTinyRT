#pragma once
#ifndef _BOX_H
#define _BOX_H

#include "Utilities.h"
#include "HittableList.h"
#include "Rect.h"

class Box : public Hittable
{
public:

	Box() {}

	Box(const Point3& p0, const Point3& p1, Material::Ptr);

	virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& rec) const override;

	virtual bool boundingBox(double t0, double t1, AABB& outputBox) const override
	{
		outputBox = AABB(boxMin, boxMax);
		return true;
	}

public:

	Point3 boxMin;
	
	Point3 boxMax;

	HittableList sides;
};

Box::Box(const Point3& p0, const Point3& p1, Material::Ptr mp)
{
	boxMin = p0;
	boxMax = p1;

	sides.add(make_shared<XYRect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), mp));
	sides.add(make_shared<XYRect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), mp));

	sides.add(make_shared<XZRect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), mp));
	sides.add(make_shared<XZRect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), mp));

	sides.add(make_shared<YZRect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), mp));
	sides.add(make_shared<YZRect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), mp));
}

bool Box::hit(const Ray& ray, double tMin, double tMax, HitRecord& rec) const
{
	return sides.hit(ray, tMin, tMax, rec);
}

#endif // !_BOX_H
