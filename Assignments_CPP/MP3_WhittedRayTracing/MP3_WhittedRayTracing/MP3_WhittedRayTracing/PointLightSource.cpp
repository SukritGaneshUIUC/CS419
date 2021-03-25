#include "PointLightSource.h"

/*
* Constructor for PointLightSource
*
* @param position The position of the PointLightSource
* @param material The material of the PointLightSource
*/
PointLightSource::PointLightSource(const Point3D& position, const Material& material) : Object(ObjectType::PointLightSource, material), position(position) {}

/*
* @return The position of the PointLightSource
*/
const Point3D& PointLightSource::getPosition() const
{
    return position;
}

/*
* Find the intersection points, if any, with a Ray3D
*
* @param ray A Ray3D.
* @param t_min The minimum t-value of the intersection.
* @param t_max The maximum t-value of the intersection.
* @param hitRecord A HitRecord struct which will store information related to the intersection (if any). Modified by function.
*
* @return The number of intersection points (1 or 0)
*/
int PointLightSource::intersection(const Ray3D& ray, const double& t_min, const double& t_max, HitRecord& hitRecord) const
{
    double intT = 0;
    Point3D intPoint;
    if (Arithmetic::ray_intersect_point(ray, position, intT, intPoint)) {
        if (intT < t_min || intT > t_max) {
            return 0;
        }
        hitRecord = HitRecord(intT, intPoint, (position - ray.getStart()).get_normalized(), getMaterial());
        return 1;
    }
    return 0;
}

/*
* Get the normal vector of the object at a specified intersection point
*
* @param intersection The intersection point
*
* @return The normal vector of the surface at intersection
*/
Vec3D PointLightSource::normal(const Point3D& intersection) const
{
    return Vec3D{ 0,0,1 };
}

/*
* Return the position of the PointLightSource
* Overridden virtual function
*
* @return The position of the PointLightSource
*/
Point3D PointLightSource::getLightPoint() const
{
    return position;
}

bool PointLightSource::generateBoundingBox(AABB3D& bb) const
{
    return false;
}
