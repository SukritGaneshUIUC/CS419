#pragma once

#include "Arithmetic.h"

#include "Object.h"

class Sphere :
    public Object
{
private:
    Point3D center;
    double radius;
    AABB3D boundingBox;
public:
    Sphere(const Point3D& center, const double& radius, const Material& material);

    const Point3D& getCenter() const;
    const double& getRadius() const;

    int intersection(const Ray3D& ray, const double& t_min, const double& t_max, HitRecord& hitRecord) const;
    Vec3D normal(const Point3D& intersection) const;
    bool generateBoundingBox(AABB3D& bb) const;

};

