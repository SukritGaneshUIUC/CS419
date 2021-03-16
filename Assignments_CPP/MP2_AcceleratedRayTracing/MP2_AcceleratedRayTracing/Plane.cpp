#include "Plane.h"

/*
* Constructor for Plane
*
* @param point Any arbitrary point on the Plane (which does not lie on the path of the specified normal vector)
* @param normal The normal vector of the Plane
* @param ambient The ambient color of the Plane
* @param diffuse The diffuse color of the Plane
* @param specular The specular color of the Plane
* @param alpha The alpha factor of the Plane
*/
Plane::Plane(const Point3D& point, const Vec3D& normal, const ColorRGB& ambient, const ColorRGB& diffuse, const ColorRGB& specular, const double& alpha) : SceneObject(ObjectType::Plane, ambient, diffuse, specular, alpha), point(point), normal_vector(normal) {}

/*
* @return The specified point on the Plane
*/
Point3D Plane::getPoint() const
{
	return point;
}

/*
* @return The normal vector of the plane
*/
Vec3D Plane::getNormal() const
{
	return normal_vector;
}

/*
* Find the intersection points, if any, with a Ray3D
*
* @param ray The potentially intersecting
* @param intPoints A vector of type Point3D, to which the function will push back any intersection points
*
* @return The number of intersection points
*/
int Plane::intersection(const Ray3D& ray, const double& t_min, const double& t_max, std::vector<double>& intTs) const
{
	double intT;
	Point3D intPoint;
	bool intersected = Arithmetic::ray_intersect_plane(point, normal_vector, ray, intT, intPoint);
	if (intersected) {
		intTs.push_back(intT);
		return 1;
	}
	return 0;
}

/*
* Find the intersection points, if any, with a Ray3D
*
* @param ray The potentially intersecting
* @param intPoints A vector of type Point3D, to which the function will push back any intersection points
*
* @return The number of intersection points
*/
int Plane::intersection(const Ray3D& ray, const double& t_min, const double& t_max, std::vector<Point3D>& intPoints) const
{
	double intT;
	Point3D intPoint;
	bool intersected = Arithmetic::ray_intersect_plane(point, normal_vector, ray, intT, intPoint);
	if (intersected) {
		intPoints.push_back(intPoint);
		return 1;
	}
	return 0;
}

/*
* Find the normal vector at a given point (irregardless of position on the surface)
* Overridden virtual function
*
* @param intersection The point on the surface at which to find the normal vector
*
* @return The normal vector (equal to the user specified normal vector)
*/
Vec3D Plane::normal(const Point3D& intersection) const
{
	return normal_vector;
}

bool Plane::generateBoundingBox(AABB3D& bb) const
{
	return false;
}
