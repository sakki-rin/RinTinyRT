#pragma once
#ifndef _PERLIN_H
#define _PERLIN_H

#include "Utilities.h"
#include "Vector3.h"

class Perlin
{
public:

	Perlin()
	{
		ranVec = new Vector3[pointCount];
		for (int i = 0; i < pointCount; ++i)
		{
			ranVec[i] = normalize(Vector3::random(-1.0, 1.0));
		}
		permX = perlinGeneratePerm();
		permY = perlinGeneratePerm();
		permZ = perlinGeneratePerm();
	}

	~Perlin()
	{
		delete[] ranVec;
		delete[] permX;
		delete[] permY;
		delete[] permZ;
	}

	double noise(const Vector3& p) const
	{
		auto u = p.x() - floor(p.x());
		auto v = p.y() - floor(p.y());
		auto w = p.z() - floor(p.z());

		//	Hermitian linear
		u = u * u * (3 - 2 * u);
		v = v * v * (3 - 2 * v);
		w = w * w * (3 - 2 * w);

		auto i = static_cast<int>(floor(p.x()));
		auto j = static_cast<int>(floor(p.y()));
		auto k = static_cast<int>(floor(p.z()));
		Vector3 c[2][2][2];
		
		//	使用三线性插值
		for (int di = 0; di < 2; di++)
		{
			for (int dj = 0; dj < 2; dj++)
			{
				for (int dk = 0; dk < 2; dk++)
				{
					c[di][dj][dk] = ranVec[permX[(i + di) & 255] ^
						permY[(j + dj) & 255] ^
						permZ[(k + dk) & 255]
					];
				}
			}
		}

		return triLinearInterp(c, u, v, w);
	}

	double turb(const Point3& p, int depth = 7) const
	{
		auto accum = 0.0;
		auto temp_p = p;
		auto weight = 1.0;

		for (int i = 0; i < depth; i++)
		{
			accum += weight * noise(temp_p);
			weight *= 0.5;
			temp_p *= 2;
		}

		return fabs(accum);
	}

private:

	static constexpr int pointCount = 256;

	Vector3* ranVec;

	int* permX;

	int* permY;

	int* permZ;

	static int* perlinGeneratePerm()
	{
		auto p = new int[pointCount];

		for (int i = 0; i < Perlin::pointCount; ++i)
		{
			p[i] = i;
		}

		permute(p, pointCount);

		return p;
	}

	static void permute(int* p, int n)
	{
		for (int i = n - 1; i > 0; i--)
		{
			int target = randomInt(0, i);
			int tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
	}

	static double triLinearInterp(Vector3 c[2][2][2], double u, double v, double w)
	{
		auto uu = u * u * (3 - 2 * u);
		auto vv = v * v * (3 - 2 * v);
		auto ww = w * w * (3 - 2 * w);
		auto accum = 0.0;

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				for (int k = 0; k < 2; k++)
				{
					Vector3 weight_v(u - i, v - j, w - k);
					accum += (i * uu + (1 - i) * (1 - uu))
						* (j * vv + (1 - j) * (1 - vv))
						* (k * ww + (1 - k) * (1 - ww))
						* dot(c[i][j][k], weight_v);
				}
			}
		}

		return accum;
	}
};

#endif // !_PERLIN_H
