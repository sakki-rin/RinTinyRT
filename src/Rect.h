#pragma once
#ifndef _RECT_H
#define _RECT_H

#include "Utilities.h"
#include "Hittable.h"
#include "Material.h"

class XYRect : public Hittable
{
public:

	XYRect() {}

	XYRect(double _x0, double _x1, double _y0, double _y1, double _k, Material::Ptr mat) 
		: x0(_x0),x1(_x1),y0(_y0),y1(_y1),k(_k),mp(mat)
	{}

	virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& rec) const override;

	virtual bool boundingBox(double t0, double t1, AABB& outputBox) const override
	{
		outputBox = AABB(Point3(x0, y0, k - 0.0001), Point3(x1, y1, k + 0.0001));
		return true;
	}

public:

	Material::Ptr mp;

	double x0, x1, y0, y1, k;
};

class XZRect : public Hittable
{
public:

	XZRect() {}

	XZRect(double _x0, double _x1, double _z0, double _z1, double _k, Material::Ptr mat)
		: x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat)
	{}

	virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& rec) const override;

	virtual bool boundingBox(double t0, double t1, AABB& outputBox) const override
	{
		outputBox = AABB(Point3(x0, k - 0.0001, z0), Point3(x1, k + 0.0001, z1));
		return true;
	}

public:

	Material::Ptr mp;

	double x0, x1, z0, z1, k;
};

class YZRect : public Hittable
{
public:

	YZRect() {}

	YZRect(double _y0, double _y1, double _z0, double _z1, double _k, Material::Ptr mat)
		: y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat)
	{}

	virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& rec) const override;

	virtual bool boundingBox(double t0, double t1, AABB& outputBox) const override
	{
		outputBox = AABB(Point3(k - 0.0001, y0, z0), Point3(k + 0.0001, y1, z1));
		return true;
	}

public:

	Material::Ptr mp;

	double y0, y1, z0, z1, k;
};

bool XYRect::hit(const Ray& ray, double tMin, double tMax, HitRecord& rec) const
{
	double t = (k - ray.origin().z()) / ray.direction().z();
	if (t<tMin || t>tMax)
	{
		return false;
	}
	double x = ray.origin().x() + t * ray.direction().x();
	double y = ray.origin().y() + t * ray.direction().y();

	if (x<x0 || x>x1 || y<y0 || y>y1)
	{
		return false;
	}

	rec.u = (x - x0) / (x1 - x0);
	rec.v = (y - y0) / (y1 - y0);
	rec.t = t;

	Vector3 outwardNormal(0, 0, 1);
	rec.setFaceNormal(ray, outwardNormal);
	rec.matptr = mp;
	rec.point = ray.at(t);

	return true;
}

bool XZRect::hit(const Ray& ray, double tMin, double tMax, HitRecord& rec) const
{
	double t = (k - ray.origin().y()) / ray.direction().y();
	if (t<tMin || t>tMax)
	{
		return false;
	}
	double x = ray.origin().x() + t * ray.direction().x();
	double z = ray.origin().z() + t * ray.direction().z();

	if (x<x0 || x>x1 || z<z0 || z>z1)
	{
		return false;
	}

	rec.u = (x - x0) / (x1 - x0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;

	Vector3 outwardNormal(0, 1, 0);
	rec.setFaceNormal(ray, outwardNormal);
	rec.matptr = mp;
	rec.point = ray.at(t);

	return true;
}

bool YZRect::hit(const Ray& ray, double tMin, double tMax, HitRecord& rec) const
{
	double t = (k - ray.origin().x()) / ray.direction().x();
	if (t<tMin || t>tMax)
	{
		return false;
	}
	double y = ray.origin().y() + t * ray.direction().y();
	double z = ray.origin().z() + t * ray.direction().z();

	if (y<y0 || y>y1 || z<z0 || z>z1)
	{
		return false;
	}

	rec.u = (y - y0) / (y1 - y0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;

	Vector3 outwardNormal(1, 0, 0);
	rec.setFaceNormal(ray, outwardNormal);
	rec.matptr = mp;
	rec.point = ray.at(t);

	return true;
}

#endif // !_RECT_H
