#pragma once

#include "Vec3D.h"

const ColorRGB DEFAULT_COLOR = WHITE_COLOR;

const ColorRGB DEFAULT_AMBIENT = PINK_COLOR;
const ColorRGB DEFAULT_DIFFUSE = PINK_COLOR;
const ColorRGB DEFAULT_SPECULAR = WHITE_COLOR;

const double DEFAULT_ALPHA = 15.0;

class Material
{
public:
	ColorRGB ambient;
	ColorRGB diffuse;
	ColorRGB specular;
	double alpha;

	Material(const ColorRGB& ambient = PINK_COLOR, const ColorRGB& diffuse = PINK_COLOR, const ColorRGB& specular = WHITE_COLOR, const double& alpha = DEFAULT_ALPHA);
};

