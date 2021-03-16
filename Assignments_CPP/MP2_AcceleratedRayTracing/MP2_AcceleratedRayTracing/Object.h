#pragma once
#include <vector>
#include <memory>

#include "Vec3D.h"
#include "Ray3D.h"
#include "AxisAlignedBoundingBox.h"
#include "Arithmetic.h"

const ColorRGB DEFAULT_COLOR = WHITE_COLOR;

enum class ObjectType { Camera, Plane, Sphere, Triangle, Cone, PointLightSource, SquareLightSource, BVHNode, None };

class Object
{
private:
	ObjectType type;
public:
	Object(ObjectType type);

	const ObjectType& getObjectType() const;

	// IMPLEMENT FOR ALL SUBCLASSES
	virtual const ColorRGB& getAmbient() const = 0;
	virtual const ColorRGB& getDiffuse() const = 0;
	virtual const ColorRGB& getSpecular() const = 0;
	virtual const double& getAlpha() const = 0;

	virtual int intersection(const Ray3D& ray, const double& t_min, const double& t_max, std::vector<double>& intTs) const = 0;
	virtual int intersection(const Ray3D& ray, const double& t_min, const double& t_max, std::vector<Point3D>& intPoints) const = 0;
	virtual Vec3D normal(const Point3D& intersection) const = 0;
	virtual bool generateBoundingBox(AABB3D& bb) const = 0;
};

struct HitRecord {
	std::shared_ptr<Object> intObject;
	double intT;
	Point3D intPoint;
	Vec3D normal;

	HitRecord(const double& intT, const Point3D& intPoint, const Vec3D& normal) {
		this->intT = intT;
		this->intPoint = intPoint;
		this->normal = normal;
	}
};
