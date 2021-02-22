#pragma once

#include "Object.h"

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
    SceneObject(const ObjectType& type, const ColorRGB& ambient=DEFAULT_COLOR, const ColorRGB& diffuse=DEFAULT_COLOR, const ColorRGB& specular=DEFAULT_COLOR, const double& alpha=DEFAULT_ALPHA);

    virtual const ColorRGB& getAmbient() const;
    virtual const ColorRGB& getDiffuse() const;
    virtual const ColorRGB& getSpecular() const;
    virtual const double& getAlpha() const;

    // IMPLEMENT FOR ALL SUBCLASSES
    virtual int intersection(const Ray3D& ray, std::vector<Point3D>& intPoints) const = 0;
    virtual Vec3D normal(const Point3D& intersection) const = 0;


};