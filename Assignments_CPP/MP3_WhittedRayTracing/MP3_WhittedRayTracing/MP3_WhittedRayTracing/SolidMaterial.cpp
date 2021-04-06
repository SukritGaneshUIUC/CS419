#include "SolidMaterial.h"

/*
* Constructor for SolidMaterial
*
* @param ambient The ambient color
* @param diffuse The diffuse color
* @param specular The specular color
* @param alpha The alpha factor
*/
SolidMaterial::SolidMaterial(const ColorRGB& ambient, const ColorRGB& diffuse, const ColorRGB& specular, const double& alpha) : Material(ambient, diffuse, specular, alpha, MaterialType::SOLID) {}
