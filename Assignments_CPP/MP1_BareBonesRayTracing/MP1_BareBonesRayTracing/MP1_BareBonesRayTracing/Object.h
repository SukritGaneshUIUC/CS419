#pragma once
#include <vector>

#include "Vec3D.h"
#include "Ray3D.h"
#include "Arithmetic.h"

const ColorRGB DEFAULT_COLOR = WHITE_COLOR;

enum class ObjectType { Camera, Plane, Sphere, Triangle, Cone, PointLightSource, SquareLightSource, None };

class Object
{
private:
	ObjectType type;
public:
	Object(ObjectType type);

	const ObjectType& getObjectType() const;

	// IMPLEMENT FOR ALL SUBCLASSES
	virtual int intersection(const Ray3D& ray, std::vector<Point3D>& intPoints) const = 0;
	virtual const ColorRGB& getDiffuse() const = 0;
	virtual const ColorRGB& getSpecular() const = 0;
};
