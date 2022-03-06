#pragma once
#ifndef _SPHERE_H
#define _SPHERE_H

#include "Hittable.h"
#include "Vector3.h"
#include "Material.h"

class Sphere : public Hittable
{
public:

	Sphere() {}

	Sphere(Point3 _center, double _radius, Material::Ptr _matPtr) 
		: mCenter(_center), mRadius(_radius), mMatPtr(_matPtr) 
	{}

	virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRec) const override;
	virtual bool boundingBox(double t0, double t1, AABB& outputBox) const override;

public:

	Point3 mCenter;
	double mRadius;
	Material::Ptr mMatPtr;

private:

	static void getUV(const Point3& p, double& u, double& v)
	{
		//	�������������(�ȣ���)������ӳ��
		//	p�ǻ��е�ĵ�λ���ϵĶ�Ӧ��
		//	u: return value [0,1] of angle �� around the Y axis from X=-1
		//	v: return value [0,1] of angle �� from Y=-1 to Y=1
		
		auto theta = acos(-p.y());	//	��ӳ�䵽theta[0,pi]
		auto phi = atan2(-p.z(), +p.x()) + pi;

		u = phi / (2 * pi);
		v = theta / pi;
	}
};

bool Sphere::hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRec) const
{
	//	���Ray��Sphere���̽�
	Vector3 OC = ray.origin() - mCenter;
	
	double a = dot(ray.direction(), ray.direction());
	double halfb = dot(ray.direction(), OC);
	double c = dot(OC, OC) - mRadius * mRadius;

	double discriminant = halfb * halfb - a * c;

	//	�н���
	if (discriminant > 0)
	{
		double temp = (-halfb - sqrt(discriminant)) / a;
		if (temp<tMax && temp>tMin)
		{
			hitRec.t = temp;
			hitRec.point = ray.at(hitRec.t);
			Vector3 outwardNormal = (hitRec.point - mCenter) / mRadius;
			hitRec.setFaceNormal(ray, outwardNormal);
			getUV(outwardNormal, hitRec.u, hitRec.v);
			hitRec.matptr = mMatPtr;
			return true;
		}
		//	������ԭ�������ڲ�ʱ����������tΪ��
		temp = (-halfb + sqrt(discriminant)) / a;
		if (temp<tMax && temp>tMin)
		{
			hitRec.t = temp;
			hitRec.point = ray.at(hitRec.t);
			Vector3 outwardNormal = (hitRec.point - mCenter) / mRadius;
			hitRec.setFaceNormal(ray, outwardNormal);
			getUV(outwardNormal, hitRec.u, hitRec.v);
			hitRec.matptr = mMatPtr;
			return true;
		}
	}
	return false;
}

bool Sphere::boundingBox(double t0, double t1, AABB& outputBox) const
{
	outputBox = AABB(mCenter - Vector3(mRadius, mRadius, mRadius),
		mCenter + Vector3(mRadius, mRadius, mRadius));
	return true;
}

class MovingSphere : public Hittable
{
public:

	MovingSphere() {}

	MovingSphere(Point3 _center0, Point3 _center1, double _radius, double t0, double t1, Material::Ptr _matPtr)
		: mCenter0(_center0), mCenter1(_center1), mRadius(_radius), time0(t0), time1(t1), mMatPtr(_matPtr)
	{}

	virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRec) const override;
	virtual bool boundingBox(double t0, double t1, AABB& outputBox) const override;

	Point3 center(double t) const;

public:

	Point3 mCenter0;
	Point3 mCenter1;	//	��ֹ����center1 = center0
	double mRadius;
	double time0;
	double time1;	//	moving
	Material::Ptr mMatPtr;
};

Point3 MovingSphere::center(double t) const
{
	return mCenter0 + ((t - time0) / (time1 - time0)) * (mCenter1 - mCenter0);	//	����ʱ������������˶�

	//	ֻ��center0��center1֮�������˶������Ը��õݹ�ʵ��
	//double exitTime = time1 - time0;
	//if (t - time0 <= time1 - time0)
	//{
	//	return center0 + ((t - time0) / (time1 - time0)) * (center1 - center0);
	//}
	//while (t - time0 > time1 - time0)
	//{
	//	t -= time1 - time0;
	//}
	//return center0 + ((1 - (t - time0)) / (time1 - time0)) * (center1 - center0);
}

bool MovingSphere::hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRec) const
{
	//	���Ray��Sphere���̽�
	Point3 curCenter = center(ray.time());
	Vector3 OC = ray.origin() - curCenter;

	double a = dot(ray.direction(), ray.direction());
	double halfb = dot(ray.direction(), OC);
	double c = dot(OC, OC) - mRadius * mRadius;

	double discriminant = halfb * halfb - a * c;

	if (discriminant > 0)
	{
		double temp = (-halfb - sqrt(discriminant)) / a;
		if (temp<tMax && temp>tMin)
		{
			hitRec.t = temp;
			hitRec.point = ray.at(hitRec.t);
			Vector3 outwardNormal = (hitRec.point - curCenter) / mRadius;
			hitRec.setFaceNormal(ray, outwardNormal);
			hitRec.matptr = mMatPtr;
			return true;
		}
		//	������ԭ�������ڲ�ʱ����������tΪ��
		temp = (-halfb + sqrt(discriminant)) / a;
		if (temp<tMax && temp>tMin)
		{
			hitRec.t = temp;
			hitRec.point = ray.at(hitRec.t);
			Vector3 outwardNormal = (hitRec.point - curCenter) / mRadius;
			hitRec.setFaceNormal(ray, outwardNormal);
			hitRec.matptr = mMatPtr;
			return true;
		}
	}
	return false;
}

bool MovingSphere::boundingBox(double t0, double t1, AABB& outputBox) const
{
	AABB box0(center(t0) - Vector3(mRadius, mRadius, mRadius), 
		center(t0) + Vector3(mRadius, mRadius, mRadius));
	AABB box1(center(t1) - Vector3(mRadius, mRadius, mRadius), 
		center(t1) + Vector3(mRadius, mRadius, mRadius));
	outputBox = surroundingBox(box0, box1);
	return true;
}

#endif // !_SPHERE_H
