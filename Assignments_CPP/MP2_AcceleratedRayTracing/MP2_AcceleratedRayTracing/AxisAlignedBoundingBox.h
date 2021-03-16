#pragma once

#include "Vec3D.h"
#include "Ray3D.h"

#include <cmath>

class AxisAlignedBoundingBox
{
private:
    Point3D minimum;
    Point3D maximum;
public:
    AxisAlignedBoundingBox();
    AxisAlignedBoundingBox(const Point3D& a, const Point3D& b);

    Point3D& min();
    Point3D& max(); 
    const Point3D& min() const;
    const Point3D& max() const;

    bool hit(const Ray3D& r, double t_min, double t_max) const;
    //inline bool hit(const Ray3D& r, double t_min, double t_max) const

    std::string toString() const;
};

using AABB = AxisAlignedBoundingBox;
using AABB3D = AxisAlignedBoundingBox;

