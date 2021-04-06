#pragma once

#include "Arithmetic.h"

#include "Object.h"

#include <algorithm>

class Triangle :
    public Object
{
private:
    Point3D vertices[3];
    Vec3D normals[3];
public:
    Triangle(const Point3D(&v)[3], const std::shared_ptr<Material>& material, const ObjectType& objectType=ObjectType::Triangle);

    const Point3D* getVertices() const;
    const Point3D& operator[](const int& idx) const;
    const Point3D& vertex0() const;
    const Point3D& vertex1() const;
    const Point3D& vertex2() const;

    virtual int intersection(const Ray3D& ray, const double& t_min, const double& t_max, HitRecord& hitRecord) const;
    Vec3D normal(const Point3D& intersection) const;

    std::string toString() const;

};

