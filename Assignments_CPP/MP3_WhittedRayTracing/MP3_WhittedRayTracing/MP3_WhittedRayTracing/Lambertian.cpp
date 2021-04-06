#include "Lambertian.h"

/*
* Default constructor for Lambertian material
* 
* @param a The material's albedo
*/
Lambertian::Lambertian(const ColorRGB& a) : albedo(a) {}

/*
* Scatters a ray upon hitting the material.
*
* @param ray The ray hitting the surface.
* @param hitRecord The HitRecord containing information about the hit.
* @param attenuation The color attenuation. Modified by function.
* @param scattered The scattered ray(s). Modified by function.
*/
bool Lambertian::scatter(const Ray3D& ray, const HitRecord& hitRecord, ColorRGB& attenuation, Ray3D& scattered) const
{
	Vec3D scatterDirection = hitRecord.normal + Arithmetic::randomUnitVec3D();

	if (scatterDirection.nearZero()) {
		scatterDirection = hitRecord.normal;
	}

	scattered = Ray3D(hitRecord.intPoint, scatterDirection);
	attenuation = albedo;
	return true;
}
