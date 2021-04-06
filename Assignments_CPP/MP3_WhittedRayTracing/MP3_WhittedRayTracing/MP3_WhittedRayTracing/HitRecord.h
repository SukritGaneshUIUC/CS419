#pragma once

#include "Vec3D.h"
#include "Ray3D.h"
#include "Arithmetic.h"

class Material;

class HitRecord 
{
public:
	bool intersected;

	double intT;
	Point3D intPoint;
	Vec3D normal;
	bool front_face;
	bool lightSource;
	std::shared_ptr<Material> material;

	HitRecord();

	void setFaceNormal(const Ray3D& ray, const Vec3D& outward_normal);
};

