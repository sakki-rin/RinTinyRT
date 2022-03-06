#pragma once
#ifndef _HITTABLE_LIST_H
#define _HITTABLE_LIST_H

#include "Hittable.h"
#include <memory>
#include <vector>

using std::make_shared;

//	��ѡ��д�����Ϲ�ϵ
class HittableList
{
public:

	HittableList() {}

	HittableList(Hittable::Ptr object) { add(object); }

	void clear()
	{
		objects.clear();
	}

	void add(Hittable::Ptr object)
	{
		objects.emplace_back(object);
	}

	bool hit(const Ray& ray, double tMin, double tMax, HitRecord& rec) const;

	bool boundingBox(double t0, double t1, AABB& outputBox) const;

public:

	std::vector<Hittable::Ptr> objects;	//	����ָ��
};

bool HittableList::hit(const Ray& ray, double tMin, double tMax, HitRecord& rec) const
{
	HitRecord curRec;
	bool hitAnything = false;
	double tClosest = tMax;
	
	for (auto& obj : objects)
	{
		if (obj->hit(ray, tMin, tClosest, curRec))
		{
			hitAnything = true;
			tClosest = curRec.t;
			rec = curRec;
		}
	}
	return hitAnything;
}

bool HittableList::boundingBox(double t0, double t1, AABB& outputBox) const
{
	if (objects.empty())
		return false;
	AABB tmpBox;
	bool firstBoundingBox = true;

	for (const auto& object : objects)
	{
		//	��Щ���������û��boundingBox
		if (!object->boundingBox(t0, t1, tmpBox))
			return false;
		outputBox = firstBoundingBox ? tmpBox : surroundingBox(outputBox, tmpBox);
		firstBoundingBox = false;
	}
	return true;
}

#endif