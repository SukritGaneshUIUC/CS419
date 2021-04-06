#pragma once
#include "Object.h"

class PointLightSource :
    public Object
{
private:
    Point3D position;
public:
    PointLightSource(const Point3D& position, const std::shared_ptr<Material>& material);

    const Point3D& getPosition() const;

    int intersection(const Ray3D& ray, const double& t_min, const double& t_max, HitRecord& hitRecord) const;
    Vec3D normal(const Point3D& intersection) const;

    Point3D getLightPoint() const;

};

