#pragma once
#include "Material.h"
class Mirror :
    public Material
{
public:
    double reflectivity;
public:
    Mirror(const ColorRGB& ambient = GRAY_COLOR, const ColorRGB& diffuse = GRAY_COLOR, const ColorRGB& specular = WHITE_COLOR, const double& alpha = DEFAULT_ALPHA, const double& reflectivity = DEFAULT_REFLECTIVITY);
    Mirror(const double& reflectivity);
};

