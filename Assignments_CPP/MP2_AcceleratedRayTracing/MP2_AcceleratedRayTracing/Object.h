#pragma once
#include <vector>

#include "Vec3D.h"
#include "Ray3D.h"
#include "AxisAlignedBoundingBox.h"
#include "Arithmetic.h"

const ColorRGB DEFAULT_COLOR = WHITE_COLOR;

enum class ObjectType { Camera, Plane, Sphere, Triangle, Cone, PointLightSource, SquareLightSource, BVHNode, None };

struct HitRecord {

};

class Object
{
private:
	ObjectType type;
public:
	Object(ObjectType type);

	const ObjectType& getObjectType() const;

	// IMPLEMENT FOR ALL SUBCLASSES
	//virtual int intersection(const Ray3D& ray, const double& t_min, const double& t_max, std::vector<double>& intTs) const = 0;
	virtual int intersection(const Ray3D& ray, const double& t_min, const double& t_max, std::vector<Point3D>& intPoints) const = 0;
	virtual bool generateBoundingBox(AABB3D& bb) const = 0;
};
