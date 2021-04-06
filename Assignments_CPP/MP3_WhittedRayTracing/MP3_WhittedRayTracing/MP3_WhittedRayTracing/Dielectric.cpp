#include "Dielectric.h"

/*
* Constructor for Dielectric
*
* @param ambient The ambient color
* @param diffuse The diffuse color
* @param specular The specular color
* @param alpha The alpha factor
* @param transmissionCoefficient The transmission coefficient
* @param refractiveIndex The index of refraction, as compared to air = 1
*/
Dielectric::Dielectric(const ColorRGB& ambient, const ColorRGB& diffuse, const ColorRGB& specular, const double& alpha, const double& transmissionCoefficient, const double& refractiveIndex) : Material(ambient, diffuse, specular, alpha, MaterialType::DIELECTRIC), transmissionCoefficient(transmissionCoefficient), refractiveIndex(refractiveIndex) {}

/*
* Constructor for Dielectric
*
* @param transmissionCoefficient The transmission coefficient
* @param refractiveIndex The index of refraction, as compared to air = 1
*/
Dielectric::Dielectric(const double& transmissionCoefficient, const double& refractiveIndex) : Material(GRAY_COLOR, GRAY_COLOR, WHITE_COLOR, DEFAULT_ALPHA, MaterialType::DIELECTRIC), transmissionCoefficient(transmissionCoefficient), refractiveIndex(refractiveIndex) {}
