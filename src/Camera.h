#pragma once
#ifndef _CAMERA_H
#define _CAMERA_H

#include "Utilities.h"

class Camera
{
public:
	
	Camera(
		Point3 camPos,
		Point3 target,
		Vector3 worldUp,
		double vfov,
		double aspect,
		double aperture,
		double focusDist,
		double t0 = 0.0,
		double t1 = 0.0
	) 
	{
		eye = camPos;
		lensRadius = aperture / 2;
		time0 = t0;
		time1 = t1;

		w = normalize(eye - target);
		u = normalize(cross(worldUp, w));
		v = cross(w, u);

		double theta = degreeToRadian(vfov);
		double viewPortHeight = tan(theta / 2) * 2.0;
		double viewPortWidth = aspect * viewPortHeight;

		horizontal = focusDist * viewPortWidth * u;
		vertical = focusDist * viewPortHeight * v;
		leftDownCorner = eye - horizontal / 2 - vertical / 2 - focusDist * w;
	}

	Ray getRay(double s, double t) const
	{
		//	Defocus blur
		Vector3 rd = lensRadius * randomInUnitDisk();
		Vector3 offset = u * rd.x() + v * rd.y();

		return Ray(eye + offset, 
			leftDownCorner + s * horizontal + t * vertical - eye - offset,
			randomDouble(time0,time1));
	}

public:

	Vector3 eye;
	Vector3 u, v, w; //	camera's axis
	double lensRadius;

	double time0;
	double time1;	//	shutter open/close time

	//	viewport
	Vector3 leftDownCorner;
	Vector3 horizontal;
	Vector3 vertical;
};

#endif // !_CAMERA_H
