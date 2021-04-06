#include "Mirror.h"

/*
* Constructor for Mirror
*
* @param ambient The ambient color
* @param diffuse The diffuse color
* @param specular The specular color
* @param alpha The alpha factor
* @param reflectivity The reflectivity factor
*/
Mirror::Mirror(const ColorRGB& ambient, const ColorRGB& diffuse, const ColorRGB& specular, const double& alpha, const double& reflectivity) : Material(ambient, diffuse, specular, alpha, MaterialType::REFLECTIVE), reflectivity(reflectivity) {}

/*
* Constructor for Mirror
*
* @param reflectivity The reflectivity factor
*/
Mirror::Mirror(const double& reflectivity) : Material(GRAY_COLOR, GRAY_COLOR, WHITE_COLOR, DEFAULT_ALPHA, MaterialType::REFLECTIVE), reflectivity(reflectivity) {}
