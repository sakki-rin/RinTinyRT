#pragma once
#ifndef _AABB_H
#define _AABB_H

#include "Ray.h"
#include "Vector3.h"
#include "Utilities.h"

class AABB
{
public:

	AABB() {}

	AABB(const Point3& _min, const Point3& _max) : minimum(_min),maximum(_max)
	{}

	Point3 min() const
	{
		return minimum;
	}

	Point3 max() const
	{
		return maximum;
	}

	//	t0、t1为时间间隔
	bool hit(const Ray& ray, double t0, double t1) const
	{
		double tEnter = t0;
		double tExit = t1;
		for (int i = 0; i < 3; ++i)
		{
			//	优化版本，略去了一些重复计算
			double invD = 1 / ray.direction()[i];
			//	min、max分别是与box的轴相交的时刻
			double tMin = (minimum[i] - ray.origin()[i]) * invD;
			double tMax = (maximum[i] - ray.origin()[i]) * invD;
			if (invD < 0.0f)
			{
				std::swap(tMin, tMax);
			}
			tEnter = tMin > tEnter ? tMin : tEnter;
			tExit = tMax < tExit ? tMax : tExit;
			if (tMax <= tMin)
			{
				return false;
			}
			//double tMin = ffmin((minimum[i] - ray.origin()[i]) / ray.direction()[i],
			//	(maximum[i] - ray.origin()[i]) / ray.direction()[i]);
			//double tMax = ffmax((minimum[i] - ray.origin()[i]) / ray.direction()[i],
			//	(maximum[i] - ray.origin()[i]) / ray.direction()[i]);
			//tEnter = ffmax(tMin, tEnter);
			//tExit = ffmin(tMax, tExit);
			//if (tEnter >= tExit || tExit < 0)
			//{
			//	return false;
			//}
		}
		return true;
	}

private:

	Point3 minimum;
	Point3 maximum;
};

AABB surroundingBox(const AABB& box0, const AABB& box1)
{
	Point3 min(ffmin(box0.min().x(), box1.min().x()),
		ffmin(box0.min().y(), box1.min().y()),
		ffmin(box0.min().z(), box1.min().z()));
	Point3 max(ffmax(box0.max().x(), box1.max().x()),
		ffmax(box0.max().y(), box1.max().y()),
		ffmax(box0.max().z(), box1.max().z()));
	return AABB(min, max);
}

#endif // !_AABB_H
