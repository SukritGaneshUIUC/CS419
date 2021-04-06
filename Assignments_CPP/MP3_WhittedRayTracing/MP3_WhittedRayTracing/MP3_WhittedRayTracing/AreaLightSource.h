#pragma once
#include "Triangle.h"
class AreaLightSource :
    public Triangle
{
private:
    std::vector<Point3D> samplePoints;
public:
    AreaLightSource(const Point3D(&v)[3], const std::shared_ptr<Material>& material);

    const std::vector<Point3D> getSamplePoints() const;
};

