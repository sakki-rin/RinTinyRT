#pragma once
#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <functional>
#include <random>

using std::shared_ptr;
using std::make_shared;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;
const double gamma = 2.2;

inline double degreeToRadian(double degrees)
{
	return degrees * pi / 180;
}

inline double ffmin(double a, double b)
{
	return a <= b ? a : b;
}

inline double ffmax(double a, double b)
{
	return a >= b ? a : b;
}

inline double randomDouble()
{
	// Returns a random real in [0,1).
	return rand() / (RAND_MAX + 1.0);
}

// Returns a random real in [0,1).
inline double C11randomDouble()
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	static std::function<double()> rand_generator =
		std::bind(distribution, generator);
	return rand_generator();
}

inline double randomDouble(double min, double max) {
    // Returns a random real in [min,max).
#ifdef CPP11
	return min + (max - min) * C11randomDouble();
#else
	return min + (max - min) * randomDouble();
#endif // CPP11
}

inline int randomInt(int min, int max)
{
	return static_cast<int>(randomDouble(min, max + 1));
}

inline double clamp(double x, double min, double max)
{
	if (x < min)
	{
		return min;
	}
	if (x > max)
	{
		return max;
	}
	return x;
}

#endif // !_UTILITIES_H
