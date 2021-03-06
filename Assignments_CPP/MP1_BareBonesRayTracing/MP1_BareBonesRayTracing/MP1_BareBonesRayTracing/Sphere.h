#pragma once

#include "Arithmetic.h"

#include "SceneObject.h"

class Sphere :
    public SceneObject
{
private:
    Point3D center;
    double radius;
public:
    Sphere(const Point3D& center, const double& radius, const ColorRGB& ambient=DEFAULT_COLOR, const ColorRGB& diffuse=DEFAULT_COLOR, const ColorRGB& specular=DEFAULT_COLOR, const double& alpha=DEFAULT_ALPHA);

    const Point3D& getCenter() const;
    const double& getRadius() const;

    int intersection(const Ray3D& ray, std::vector<Point3D>& intPoints) const;
    Vec3D normal(const Point3D& intersection) const;

};

