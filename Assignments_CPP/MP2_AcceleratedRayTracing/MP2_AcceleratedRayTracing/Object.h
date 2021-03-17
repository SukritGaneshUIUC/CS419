#pragma once
#include <vector>
#include <memory>

#include "Vec3D.h"
#include "Ray3D.h"
#include "AxisAlignedBoundingBox.h"
#include "Arithmetic.h"

const ColorRGB DEFAULT_COLOR = WHITE_COLOR;

enum class ObjectType { Camera, Plane, Sphere, Triangle, Cone, PointLightSource, SquareLightSource, BVHNode, None };

struct Material {
	ColorRGB ambient;
	ColorRGB diffuse;
	ColorRGB specular;
	double alpha;

	Material() {
		this->ambient = DEFAULT_COLOR;
		this->diffuse = DEFAULT_COLOR;
		this->specular = DEFAULT_COLOR;
		this->alpha = 5.0;
	};

	Material(const ColorRGB& ambient, const ColorRGB& diffuse, const ColorRGB& specular, const double& alpha) {
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->alpha = alpha;
	}
};

struct HitRecord {
	double intT;
	Point3D intPoint;
	Vec3D normal;
	Material material;

	HitRecord() {}

	HitRecord(const double& intT, const Point3D& intPoint, const Vec3D& normal) {
		this->intT = intT;
		this->intPoint = intPoint;
		this->normal = normal;
		this->material = Material();
	}

	HitRecord(const double& intT, const Point3D& intPoint, const Vec3D& normal, const Material& material) {
		this->intT = intT;
		this->intPoint = intPoint;
		this->normal = normal;
		this->material = material;
	}

	HitRecord(const double& intT, const Point3D& intPoint, const Vec3D& normal, const ColorRGB& ambient, const ColorRGB& diffuse, const ColorRGB& specular, const double& alpha) {
		this->intT = intT;
		this->intPoint = intPoint;
		this->normal = normal;
		this->material = Material(ambient, diffuse, specular, alpha);
	}
};

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

	virtual int intersection(const Ray3D& ray, const double& t_min, const double& t_max, HitRecord& hitRecord) const = 0;
	virtual Vec3D normal(const Point3D& intersection) const = 0;
	virtual bool generateBoundingBox(AABB3D& bb) const = 0;
};
