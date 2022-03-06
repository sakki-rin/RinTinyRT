#pragma once
#ifndef _COLOR_H
#define _COLOR_H

#include "Vector3.h"
#include "Utilities.h"

#include <fstream>

void writeColor(std::ofstream& out, const Color& pixelColor, int samples)
{
	double r = pixelColor[0];
	double g = pixelColor[1];
	double b = pixelColor[2];

	//	re::calculate pixel color
	double scale = 1.0 / samples;
	r *= scale;
	g *= scale;
	b *= scale;

	//	use gamma corretion
	double w = 1.0 / gamma;
	r = pow(r, w);
	g = pow(g, w);
	b = pow(b, w);

	// Write the translated [0,255] value of each color component
	out << static_cast<int>(255.999 * clamp(r, 0.0, 1.0)) << ' '
		<< static_cast<int>(255.999 * clamp(g, 0.0, 1.0)) << ' '
		<< static_cast<int>(255.999 * clamp(b, 0.0, 1.0)) << '\n';
}

#endif // !_COLOR_H