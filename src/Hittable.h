#pragma once
#ifndef _HITTABLE_H
#define _HITTABLE_H

#include "Ray.h"
#include "AABB.h"

class Material;

struct HitRecord
{
	Point3 point;	//	�ཻ����
	Point3 normal;	//	���㴦������
	shared_ptr<Material> matptr;	//pImpl����
	double t;		//	tֵ
	double u;		//	�洢���е����������
	double v;		
	bool frontFace;	//	����������Ϊ�����ڲ�

	//	��������Զ�����䷽���෴����������������ԭ��ͬ��
	void setFaceNormal(const Ray& ray, const Vector3& outwardNormal)
	{
		frontFace = dot(ray.direction(), outwardNormal) < 0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};

class Hittable
{
public:
	using Ptr = std::shared_ptr<Hittable>;
	//	t_min < t < t_max
	//	ֻ����������ԭ������Ľ���ķ�����
	virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRec) const = 0;
	virtual bool boundingBox(double t0, double t1, AABB& outputBox) const = 0;
};


class Translate : public Hittable
{
public:

	Translate(Hittable::Ptr p, const Vector3& displacement)
		: objPtr(p),offset(displacement) {}

	virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRec) const override;

	virtual bool boundingBox(double t0, double t1, AABB& outputBox) const override;

public:

	Hittable::Ptr objPtr;

	Vector3 offset;
};

bool Translate::hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRec) const
{
	//	
	Ray movedRay(ray.origin() - offset, ray.direction(), ray.time());
	if (!objPtr->hit(movedRay, tMin, tMax, hitRec))
	{
		return false;
	}

	hitRec.point += offset;
	hitRec.setFaceNormal(movedRay, hitRec.normal);
	return true;
}

bool Translate::boundingBox(double t0, double t1, AABB& outputBox) const
{
	//	��ȡ����ƽ��ǰ��boundingBox
	if (!objPtr->boundingBox(t0, t1, outputBox))
	{
		return false;
	}
	//	����ƽ�ƺ��boundingBox
	outputBox = AABB(outputBox.min() + offset, outputBox.max() + offset);
	return true;
}

class RotateY : public Hittable
{
public:

	RotateY(Hittable::Ptr p, double angle);

	virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRec) const override;

	virtual bool boundingBox(double t0, double t1, AABB& outputBox) const override
	{
		outputBox = box;
		return hasBox;
	}

public:

	Hittable::Ptr objPtr;

	double theta;

	bool hasBox;

	AABB box;
};

RotateY::RotateY(Hittable::Ptr p, double angle) : objPtr(p), theta(degreeToRadian(angle))
{
	double sinTheta = sin(theta);
	double cosTheta = cos(theta);

	hasBox = objPtr->boundingBox(0, 1, box);

	Point3 min(infinity, infinity, infinity);
	Point3 max(-infinity, -infinity, -infinity);

	//	����bbox�˸�������ת���λ�ã������¼���bbox
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			for (int k = 0; k < 2; ++k)
			{
				double x = i * box.max().x() + (1 - i) * box.min().x();
				double y = j * box.max().y() + (1 - j) * box.min().y();
				double z = k * box.max().z() + (1 - k) * box.min().z();

				double newX = cosTheta * x + sinTheta * z;
				double newZ = -sinTheta * x + cosTheta * z;

				Vector3 temp(newX, y, newZ);

				for (int c = 0; c < 3; ++c)
				{
					min[c] = ffmin(min[c], temp[c]);
					max[c] = ffmax(max[c], temp[c]);
				}
			}
		}
	}

	box = AABB(min, max);
}

bool RotateY::hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRec) const
{
	double sinTheta = sin(theta);
	double cosTheta = cos(theta);

	Point3 origin = ray.origin();
	Vector3 dir = ray.direction();

	origin[0] = cosTheta * ray.origin()[0] - sinTheta * ray.origin()[2];
	origin[2] = sinTheta * ray.origin()[0] + cosTheta * ray.origin()[2];

	dir[0] = cosTheta * ray.direction()[0] - sinTheta * ray.direction()[2];
	dir[2] = sinTheta * ray.direction()[0] + cosTheta * ray.direction()[2];

	Ray rotatedRay(origin, dir, ray.time());

	if (!objPtr->hit(rotatedRay, tMin, tMax, hitRec))
	{
		return false;
	}

	Point3 p = hitRec.point;
	Vector3 normal = hitRec.normal;

	p[0] = cosTheta * hitRec.point[0] + sinTheta * hitRec.point[2];
	p[2] = -sinTheta * hitRec.point[0] + cosTheta * hitRec.point[2];

	normal[0] = cosTheta * hitRec.normal[0] + sinTheta * hitRec.normal[2];
	normal[2] = -sinTheta * hitRec.normal[0] + cosTheta * hitRec.normal[2];

	hitRec.point = p;
	hitRec.setFaceNormal(rotatedRay, normal);

	return true;
}

#endif // !_HITTABLE_H
