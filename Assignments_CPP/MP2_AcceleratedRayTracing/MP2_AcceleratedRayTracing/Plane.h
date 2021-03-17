#pragma once

#include "Arithmetic.h"

#include "SceneObject.h"

class Plane :
    public SceneObject, std::enable_shared_from_this<SceneObject>
{
private:
    Point3D point;
    Vec3D normal_vector;
public:
    Plane(const Point3D& point, const Vec3D& normal, const ColorRGB& ambient = DEFAULT_COLOR, const ColorRGB& diffuse = DEFAULT_COLOR, const ColorRGB& specular = DEFAULT_COLOR, const double& alpha = DEFAULT_ALPHA);

    Point3D getPoint() const;
    Vec3D getNormal() const;

    int intersection(const Ray3D& ray, const double& t_min, const double& t_max, HitRecord& hitRecord) const;
    Vec3D normal(const Point3D& intersection) const;
    bool generateBoundingBox(AABB3D& bb) const;
};

