#include "Material.h"

/*
* Constructor for Material
*
* @param ambient The ambient color
* @param diffuse The diffuse color
* @param specular The specular color
* @param alpha The alpha factor
* @param materialType The material type
*/
Material::Material(const ColorRGB& ambient, const ColorRGB& diffuse, const ColorRGB& specular, const double& alpha, const MaterialType& materialType) {
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->alpha = alpha;
	this->materialType = materialType;
}

/*
* Scatters a ray upon hitting the material.
* 
* @param ray The ray hitting the surface.
* @param hitRecord The HitRecord containing information about the hit.
* @param attenuation The color attenuation. Modified by function.
* @param scattered The scattered ray(s). Modified by function.
*/
bool Material::scatter(const Ray3D& ray, const HitRecord& hitRecord, ColorRGB& attenuation, Ray3D& scattered) const
{
	return false;
}
