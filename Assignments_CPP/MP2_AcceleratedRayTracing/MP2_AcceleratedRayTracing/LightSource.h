#pragma once
#include "Object.h"

constexpr double MAX_BRIGHTNESS = 1.0;
constexpr double DEFAULT_BRIGHTNESS = 0.5;

class LightSource :
    public Object
{
private:
    double brightness;
    ColorRGB diffuse;
    ColorRGB specular;
public:
    LightSource(const ObjectType& type, const ColorRGB& diffuse = DEFAULT_COLOR, const ColorRGB& specular = DEFAULT_COLOR, const double& brightness = DEFAULT_BRIGHTNESS);

    const double& getBrightness() const;
    const ColorRGB& getAmbient() const;
    const ColorRGB& getDiffuse() const;
    const ColorRGB& getSpecular() const;
    const double& getAlpha() const;

    // IMPLEMENT ON ALL SUBCLASSES
    virtual int intersection(const Ray3D& ray, const double& t_min, const double& t_max, HitRecord& hitRecord) const = 0;
    virtual Vec3D normal(const Point3D& intersection) const = 0;
    virtual bool generateBoundingBox(AABB3D& bb) const = 0;

    virtual Point3D getLightPoint() const = 0;    // return a point on the light source (randomly or stratified sample using static variable), useful for ray tracing

};

