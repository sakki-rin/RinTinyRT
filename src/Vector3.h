#pragma once
#pragma once

#include "Utilities.h"

#include <iostream>

class Vector3
{
public:

	Vector3() : e{ 0.0,0.0,0.0 } {}

	Vector3(double e0, double e1, double e2) : e{ e0,e1,e2 } {}

	Vector3(const Vector3& v)
	{
		*this = v;
	}

	double x() const
	{
		return e[0];
	}

	double y() const
	{
		return e[1];
	}

	double z() const
	{
		return e[2];
	}

	Vector3 operator-() const
	{
		return Vector3(-e[0], -e[1], -e[2]);
	}

	double operator[](int i) const
	{
		return e[i];
	}

	double& operator[](int i)
	{
		return e[i];
	}

	Vector3& operator=(const Vector3& v)
	{
		e[0] = v.e[0];
		e[1] = v.e[1];
		e[2] = v.e[2];
		return *this;
	}

	Vector3& operator+=(const Vector3& v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	Vector3& operator*=(const double t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	Vector3& operator/=(const double t)
	{
		return *this *= 1 / t;
	}

	double length() const
	{
		return std::sqrt(lengthSquared());
	}

	double lengthSquared() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	inline static Vector3 random()
	{
		return Vector3(C11randomDouble(), C11randomDouble(), C11randomDouble());
	}

	inline static Vector3 random(double min, double max)
	{
		return Vector3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
	}

	inline bool nearZero()
	{
		const auto limit = 1e-8;
		return (fabs(e[0]) < limit) && (fabs(e[1]) < limit) && (fabs(e[2]) < limit);
	}

public:

	double e[3];
};

using Point3 = Vector3;
using Color = Vector3;


inline std::ostream& operator<<(std::ostream& out, const Vector3& v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vector3 operator+(const Vector3& u, const Vector3& v)
{
	return Vector3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vector3 operator-(const Vector3& u, const Vector3& v)
{
	return Vector3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vector3 operator*(const Vector3& u, const Vector3& v)
{
	return Vector3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vector3 operator*(double t, const Vector3& v)
{
	return Vector3(t * v.e[0], t * v.e[1], t * v.e[2]);
}


inline Vector3 operator*(const Vector3& v, double t)
{
	return t * v;
}

inline Vector3 operator/(const Vector3& v, double t)
{
	return (1 / t) * v;
}

inline double dot(const Vector3& u, const Vector3& v)
{
	return u.e[0] * v.e[0] +
		u.e[1] * v.e[1] +
		u.e[2] * v.e[2];
}

inline Vector3 cross(const Vector3& u, const Vector3& v)
{
	return Vector3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

//inline vec3& normalize(vec3& v)
//{
//	v /= v.length();
//	return v;
//}

inline Vector3 normalize(const Vector3& v)
{
	return v / v.length();
}

//	generate inside point of unit sphere
Vector3 randomVecInUnitSphere()
{
	Vector3 vec;
	do
	{
		vec = Vector3::random(-1, 1);
	} while (vec.lengthSquared() >= 1);
	return vec;
}

//	optimize lambertian scatter, only points "on" the sphere
Vector3 randomUnitVector()
{
	//return normalize(randomVecInUnitSphere());
	//	just try lambda
	std::function<Vector3()> foo = [&]()->Vector3 {
		Vector3 vec;
		do
		{
			vec = Vector3::random(-1, 1);
		} while (vec.lengthSquared() >= 1);
		return vec; 
	};
	return foo();
}

Vector3 randomInHemiSphere(const Vector3& normal)
{
	Vector3 in_unit_sphere = randomVecInUnitSphere();
	if (dot(in_unit_sphere, normal) > 0.0)
	{
		return in_unit_sphere;
	}
	else
	{
		return -in_unit_sphere;
	}
}

Vector3 randomInUnitDisk()
{
	while (true)
	{
		Vector3 point = Vector3(randomDouble(-1, 1), randomDouble(-1, 1), 0);
		if (point.lengthSquared() >= 1)
			continue;
		return point;
	}
}

Vector3 reflect(const Vector3& v, const Vector3& n)
{
	return v - 2 * dot(v, n) * n;
}

Vector3 refract(const Vector3& RayIn, const Vector3& n, double etaiFracEtat)
{
	double cosTheta = dot(-RayIn, n);
	Vector3 rayParallel = etaiFracEtat * (RayIn + cosTheta * n);
	Vector3 rayPerp = -sqrt(1.0 - rayParallel.lengthSquared()) * n;
	return rayParallel + rayPerp;
}