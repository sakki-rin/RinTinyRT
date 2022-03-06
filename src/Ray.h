#pragma once
#ifndef _RAY_H
#define _RAY_H

#include "Vector3.h"

class Ray
{
public:

	Ray() {}

	Ray(const Vector3& _origin, const Vector3& _dir, double time = 0.0) :
		mOrigin(_origin), mDirection(_dir), mTime(time)
	{}

	Vector3 origin() const
	{
		return mOrigin;
	}

	Vector3 direction() const
	{
		return mDirection;
	}

	double time() const
	{
		return mTime;
	}

	Vector3 at(double t) const
	{
		return mOrigin + t * mDirection;
	}

public:

	Vector3 mOrigin;
	Vector3 mDirection;
	double mTime;	//	the ray's existence time
};

#endif // !_RAY_H