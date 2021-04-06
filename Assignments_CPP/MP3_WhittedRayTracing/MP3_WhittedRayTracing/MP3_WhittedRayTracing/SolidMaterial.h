#pragma once
#include "Material.h"
class SolidMaterial :
    public Material
{
public:
    SolidMaterial(const ColorRGB& ambient = PINK_COLOR, const ColorRGB& diffuse = PINK_COLOR, const ColorRGB& specular = WHITE_COLOR, const double& alpha = DEFAULT_ALPHA);
};

