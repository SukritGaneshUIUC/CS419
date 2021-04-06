#pragma once

#include "Arithmetic.h"

#include "Object.h"

class Sphere :
    public Object
{
private:
    Point3D center;
    double radius;
public:
    Sphere(const Point3D& center, const double& radius, const std::shared_ptr<Material>& material);

    const Point3D& getCenter() const;
    const double& getRadius() const;

    int intersection(const Ray3D& ray, const double& t_min, const double& t_max, HitRecord& hitRecord) const;
    Vec3D normal(const Point3D& intersection) const;

};

