#pragma once
#include <vector>
#include <memory>

#include "HitRecord.h"
#include "Material.h"
#include "Vec3D.h"
#include "Ray3D.h"
#include "Arithmetic.h"

enum class ObjectType { Plane, Sphere, Triangle, PointLightSource, AreaLightSource, None };

class Object
{
private:
	ObjectType type;
	std::shared_ptr<Material> material;
public:
	Object(const ObjectType& type, const std::shared_ptr<Material>& material);

	const ObjectType& getObjectType() const;
	const std::shared_ptr<Material>& getMaterial() const;

	// IMPLEMENT FOR ALL SUBCLASSES
	virtual int intersection(const Ray3D& ray, const double& t_min, const double& t_max, HitRecord& hitRecord) const = 0;
	virtual Vec3D normal(const Point3D& intersection) const = 0;
};
