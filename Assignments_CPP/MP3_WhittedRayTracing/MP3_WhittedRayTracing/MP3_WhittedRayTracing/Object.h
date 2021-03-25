#pragma once
#include <vector>
#include <memory>

#include "Material.h"
#include "Vec3D.h"
#include "Ray3D.h"
#include "AxisAlignedBoundingBox.h"
#include "Arithmetic.h"

enum class ObjectType { Camera, Plane, Sphere, Triangle, Cone, PointLightSource, SquareLightSource, BVHNode, None };

struct HitRecord {
	double intT;
	Point3D intPoint;
	Vec3D normal;
	Material material;

	/*
	* Default constructor for HitRecord
	*/
	HitRecord(): intT(0), intPoint(BLACK_COLOR), normal(BLACK_COLOR), material(Material()) {}


	/*
	* Constructor for HitRecord
	*
	* @param intT The intersection t-value
	* @param intPoint The intersection point
	* @param normal The normal of the intersection surface at the intersection point
	*/
	HitRecord(const double& intT, const Point3D& intPoint, const Vec3D& normal) {
		this->intT = intT;
		this->intPoint = intPoint;
		this->normal = normal;
		this->material = Material();
	}


	/*
	* Constructor for HitRecord
	*
	* @param intT The intersection t-value
	* @param intPoint The intersection point
	* @param normal The normal of the intersected surface at the intersection point
	* @param material The material of the intersected surface
	*/
	HitRecord(const double& intT, const Point3D& intPoint, const Vec3D& normal, const Material& material) {
		this->intT = intT;
		this->intPoint = intPoint;
		this->normal = normal;
		this->material = material;
	}
};

class Object
{
private:
	ObjectType type;
	Material material;
public:
	Object(const ObjectType& type, const Material& material);

	const ObjectType& getObjectType() const;
	const Material& getMaterial() const;

	// IMPLEMENT FOR ALL SUBCLASSES
	virtual int intersection(const Ray3D& ray, const double& t_min, const double& t_max, HitRecord& hitRecord) const = 0;
	virtual Vec3D normal(const Point3D& intersection) const = 0;
	virtual bool generateBoundingBox(AABB3D& bb) const = 0;
};
