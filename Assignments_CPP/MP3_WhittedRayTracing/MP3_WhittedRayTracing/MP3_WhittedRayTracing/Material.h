#pragma once

#include "Vec3D.h"
#include "Ray3D.h"
#include "HitRecord.h"
#include "Arithmetic.h"

const ColorRGB DEFAULT_COLOR = WHITE_COLOR;

const ColorRGB DEFAULT_AMBIENT = PINK_COLOR;
const ColorRGB DEFAULT_DIFFUSE = PINK_COLOR;
const ColorRGB DEFAULT_SPECULAR = WHITE_COLOR;
const double DEFAULT_ALPHA = 15.0;

const double DEFAULT_REFLECTIVITY = 0.9;
const double DEFAULT_TRANSMISSION_COEFFICIENT = 0.9;
const double DEFAULT_REFRACTIVE_INDEX = 1.3;

enum class MaterialType {REFLECTIVE, SOLID, DIELECTRIC};

struct HitRecord;

class Material
{
public:
	ColorRGB ambient;
	ColorRGB diffuse;
	ColorRGB specular;
	double alpha;
	MaterialType materialType;

	Material(const ColorRGB& ambient = PINK_COLOR, const ColorRGB& diffuse = PINK_COLOR, const ColorRGB& specular = WHITE_COLOR, const double& alpha = DEFAULT_ALPHA, const MaterialType& materialType = MaterialType::SOLID);

	virtual bool scatter(const Ray3D& ray, const HitRecord& hitRecord, ColorRGB& attenuation, Ray3D& scattered) const;
};

