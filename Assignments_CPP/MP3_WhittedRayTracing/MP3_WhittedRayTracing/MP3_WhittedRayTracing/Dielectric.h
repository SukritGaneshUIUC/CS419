#pragma once
#include "Material.h"
class Dielectric :
    public Material
{
public:
    double transmissionCoefficient;
    double refractiveIndex;
public:
    Dielectric(const ColorRGB& ambient = GRAY_COLOR, const ColorRGB& diffuse = GRAY_COLOR, const ColorRGB& specular = WHITE_COLOR, const double& alpha = DEFAULT_ALPHA, const double& transmissionCoefficient = DEFAULT_TRANSMISSION_COEFFICIENT, const double& refractiveIndex = DEFAULT_REFRACTIVE_INDEX);
    Dielectric(const double& transmissionCoefficient, const double& refractiveIndex);
};

