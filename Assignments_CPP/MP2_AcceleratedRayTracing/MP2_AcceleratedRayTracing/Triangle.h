#pragma once

#include "Arithmetic.h"

#include "SceneObject.h"

#include <algorithm>

class Triangle :
    public SceneObject
{
private:
    Point3D vertices[3];
    Vec3D normalVector;
public:
    Triangle(const Point3D(&v)[3], const ColorRGB& ambient = DEFAULT_COLOR, const ColorRGB& diffuse = DEFAULT_COLOR, const ColorRGB& specular = DEFAULT_COLOR, const double& alpha = DEFAULT_ALPHA);

    const Point3D* getVertices() const;
    const Point3D& operator[](const int& idx) const;
    const Point3D& vertex0() const;
    const Point3D& vertex1() const;
    const Point3D& vertex2() const;

    void setNormal(const Vec3D& normalVector);

    //int intersection(const Ray3D& ray, const double& t_min, const double& t_max, std::vector<double>& intTs) const;
    int intersection(const Ray3D& ray, const double& t_min, const double& t_max, std::vector<Point3D>& intPoint) const;
    Vec3D normal(const Point3D& intersection) const;
    bool generateBoundingBox(AABB3D& bb) const;

};

