#pragma once
#ifndef _TEXTURE_H
#define _TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION

#include "Vector3.h"
#include "Perlin.h"
#include "stb_image.h"
#include "stb_image_write.h"

class Texture
{
public:
	using Ptr = std::shared_ptr<Texture>;

public:
	virtual Color texValue(double u, double v, const Point3& point) const = 0;
};

//	静态rgb作为贴图
class ConstTexture : public Texture
{
public:

	ConstTexture() {}

	ConstTexture(Color _color) : mColor(_color) {}

	ConstTexture(double red, double green, double blue) : mColor(Color(red,green,blue)) {}

	virtual Color texValue(double u, double v, const Point3& point) const override
	{
		return mColor;
	}

public:

	Color mColor;
};

//	棋盘纹纹理
class CheckerTexture : public Texture
{
public:

	CheckerTexture() {}

	CheckerTexture(Texture::Ptr _even, Texture::Ptr _odd)
		: even(_even),odd(_odd) {}

	CheckerTexture(const Color& c1, const Color& c2) 
		: even(make_shared<ConstTexture>(c1)), odd(make_shared<ConstTexture>(c2)) {}

	virtual Color texValue(double u, double v, const Point3& point) const override
	{
		auto sines = sin(point.x() * 10) * sin(point.y() * 10) * sin(point.z() * 10);
		if (sines < 0)
		{
			return odd->texValue(u, v, point);
		}
		else
		{
			return even->texValue(u, v, point);
		}
	}

public:

	Texture::Ptr even;

	Texture::Ptr odd;
};

class NoiseTexture : public Texture
{
public:

	NoiseTexture() {}

	NoiseTexture(double sc) : scale(sc) {}

	virtual Color texValue(double u, double v, const Vector3& p) const override
	{
		return Color(1.0, 1.0, 1.0) * 0.5 * (1 + sin(scale * p.z() + 10 * noise.turb(p)));
	}

private:

	Perlin noise;

	double scale;
};

class ImageTexture : public Texture
{
public:

	ImageTexture() : data(nullptr), width(0), height(0), bytesPerScanline(0)
	{
	}

	ImageTexture(const char* filename)
	{
		int componentsPerPixel = bytesPerPixel;

		data = stbi_load(filename, &width, &height, &componentsPerPixel, componentsPerPixel);

		if (!data)
		{
			std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
			width = height = 0;
		}

		bytesPerScanline = bytesPerPixel * width;
	}

	~ImageTexture()
	{
		delete data;
	}

	virtual Color texValue(double u, double v, const Point3& p) const override
	{
		//	如果没有加载Image data
		if (data == nullptr)
		{
			return Color(0.0, 1.0, 1.0);
		}

		u = clamp(u, 0.0, 1.0);
		//	flip V to image coordinates
		v = 1.0 - clamp(v, 0.0, 1.0);

		int col = static_cast<int>(u * width);
		int row = static_cast<int>(v * height);

		col >= width ? width - 1 : col;
		row >= height ? height - 1 : row;

		double colorScale = 1.0 / 255.0;
		auto pixel = data + row * bytesPerScanline + col * bytesPerPixel;

		return Color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
	}

public:

	static const int bytesPerPixel = 3;

	unsigned char* data;

	int width;

	int height;

	int bytesPerScanline;
};

#endif // !_TEXTURE_H
