#include "PointLightSource.h"

/*
* Constructor for PointLightSource
*
* @param position The position of the PointLightSource
* @param diffuse The diffuse color of the PointLightSource
* @param specular The specular color of the PointLightSource
* @param brightness The brightness of the PointLightSource
*/
PointLightSource::PointLightSource(const Point3D& position, const ColorRGB& diffuse, const ColorRGB& specular, float brightness) : LightSource(ObjectType::PointLightSource, diffuse, specular, brightness), position(position) {}

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
* @param ray Potentially intersecting ray.
* @param HitRecord Stores the intersection records. Modified by function.
*
* @return Number of intersection points: 1 upon successful intersection, 0 otherwise
*/
int PointLightSource::intersection(const Ray3D& ray, const double& t_min, const double& t_max, HitRecord& hitRecord) const
{
    double intT = 0;
    Point3D intPoint;
    if (Arithmetic::ray_intersect_point(ray, position, intT, intPoint)) {
        if (intT < t_min || intT > t_max) {
            return 0;
        }
        hitRecord = HitRecord(intT, intPoint, (position - ray.getStart()).get_normalized());
        return 1;
    }
    return 0;
}

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
