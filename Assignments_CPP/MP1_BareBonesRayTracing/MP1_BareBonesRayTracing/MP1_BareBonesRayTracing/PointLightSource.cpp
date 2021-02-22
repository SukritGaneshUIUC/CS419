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
* @param ray The potentially intersecting 
* @param intPoints A vector of type Point3D, to which the function will push back any intersection points
*
* @return The number of intersection points
*/
int PointLightSource::intersection(const Ray3D& ray, std::vector<Point3D>& intPoints) const
{
    if (Arithmetic::ray_intersect_point(ray, position)) {
        intPoints.push_back(position);
        return 1;
    }
    return 0;
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
