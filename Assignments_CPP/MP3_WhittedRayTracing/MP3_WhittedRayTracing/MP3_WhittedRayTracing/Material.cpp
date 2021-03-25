#include "Material.h"

/*
* Constructor for Material
*
* @param ambient The ambient color
* @param diffuse The diffuse color
* @param specular The specular color
* @param alpha The alpha factor
*/
Material::Material(const ColorRGB& ambient, const ColorRGB& diffuse, const ColorRGB& specular, const double& alpha) {
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->alpha = alpha;
}