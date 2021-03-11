#pragma once

#include "Object.h"
#include "AxisAlignedBoundingBox.h"

constexpr double DEFAULT_ALPHA = 15.0;

class SceneObject :
    public Object
{
private:
    ColorRGB ambient;
    ColorRGB diffuse;
    ColorRGB specular;
    double alpha;

public:
    SceneObject(const ObjectType& type, const ColorRGB& ambient = DEFAULT_COLOR, const ColorRGB& diffuse = DEFAULT_COLOR, const ColorRGB& specular = DEFAULT_COLOR, const double& alpha = DEFAULT_ALPHA);

    const ColorRGB& getAmbient() const;
    const ColorRGB& getDiffuse() const;
    const ColorRGB& getSpecular() const;
    const double& getAlpha() const;

    // IMPLEMENT FOR ALL SUBCLASSES
    //virtual int intersection(const Ray3D& ray, const double& t_min, const double& t_max, std::vector<double>& intTs) const = 0;
    virtual int intersection(const Ray3D& ray, const double& t_min, const double& t_max, std::vector<Point3D>& intPoints) const = 0;
    virtual Vec3D normal(const Point3D& intersection) const = 0;
    virtual bool generateBoundingBox(AABB3D& bb) const = 0;


};