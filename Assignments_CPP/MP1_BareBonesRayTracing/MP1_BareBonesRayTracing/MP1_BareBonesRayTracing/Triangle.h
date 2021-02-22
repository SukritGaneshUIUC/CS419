#pragma once

#include "Arithmetic.h"

#include "SceneObject.h"

class Triangle :
    public SceneObject
{
private:
    Point3D vertices[3];
public:
    Triangle(const Point3D(&v)[3], const ColorRGB& ambient = DEFAULT_COLOR, const ColorRGB& diffuse = DEFAULT_COLOR, const ColorRGB& specular = DEFAULT_COLOR, const double& alpha = DEFAULT_ALPHA);

    const Point3D * getVertices() const;
    const Point3D& vertex0() const;
    const Point3D& vertex1() const;
    const Point3D& vertex2() const;

    int intersection(const Ray3D& ray, std::vector<Point3D>& intPoints) const;
    Vec3D normal(const Point3D& intersection) const;

};

