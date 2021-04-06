#pragma once

#include "Arithmetic.h"

#include "Object.h"

class Plane :
    public Object
{
private:
    Point3D point;
    Vec3D normal_vector;
public:
    Plane(const Point3D& point, const Vec3D& normal, const std::shared_ptr<Material>& material);

    Point3D getPoint() const;
    Vec3D getNormal() const;

    int intersection(const Ray3D& ray, const double& t_min, const double& t_max, HitRecord& hitRecord) const;
    Vec3D normal(const Point3D& intersection) const;
};

