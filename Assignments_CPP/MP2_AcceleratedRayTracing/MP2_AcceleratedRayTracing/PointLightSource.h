#pragma once
#include "LightSource.h"

class PointLightSource :
    public LightSource
{
private:
    Point3D position;
public:
    PointLightSource(const Point3D& position, const ColorRGB& diffuse = DEFAULT_COLOR, const ColorRGB& specular = DEFAULT_COLOR, float brightness = DEFAULT_BRIGHTNESS);

    const Point3D& getPosition() const;

    //int intersection(const Ray3D& ray, const double& t_min, const double& t_max, std::vector<double>& intTs) const;
    int intersection(const Ray3D& ray, const double& t_min, const double& t_max, std::vector<Point3D>& intPoints) const;
    Point3D getLightPoint() const;
    bool generateBoundingBox(AABB3D& bb) const;

};

