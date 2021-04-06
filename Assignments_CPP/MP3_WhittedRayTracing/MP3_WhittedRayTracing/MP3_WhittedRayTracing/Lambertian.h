#pragma once
#include "Material.h"

class Lambertian :
    public Material
{
public:
    ColorRGB albedo;

    Lambertian(const ColorRGB& a);

    virtual bool scatter(const Ray3D& ray, const HitRecord& hitRecord, ColorRGB& attenuation, Ray3D& scattered) const;
};

