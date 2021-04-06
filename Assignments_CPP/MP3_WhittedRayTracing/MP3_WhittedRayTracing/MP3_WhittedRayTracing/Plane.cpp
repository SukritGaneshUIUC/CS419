#include "Plane.h"

/*
* Constructor for Plane
*
* @param point Any arbitrary point on the Plane (which does not lie on the path of the specified normal vector)
* @param normal The normal vector of the Plane
* @param material The Material of the Plane
*/
Plane::Plane(const Point3D& point, const Vec3D& normal, const std::shared_ptr<Material>& material) : Object(ObjectType::Plane, material), point(point), normal_vector(normal) {
	(this->normal_vector).normalize();
}

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
* @param ray A Ray3D.
* @param t_min The minimum t-value of the intersection.
* @param t_max The maximum t-value of the intersection.
* @param hitRecord A HitRecord struct which will store information related to the intersection (if any). Modified by function.
*
* @return The number of intersection points (1 or 0)
*/
int Plane::intersection(const Ray3D& ray, const double& t_min, const double& t_max, HitRecord& hitRecord) const
{
	double intT;
	Point3D intPoint;
	bool intersected = Arithmetic::ray_intersect_plane(point, normal_vector, ray, intT, intPoint);
	if (intersected) {
		if (intT < t_min || intT > t_max) {
			return 0;
		}
		hitRecord.intersected = true;
		hitRecord.intT = intT;
		hitRecord.intPoint = intPoint;
		hitRecord.setFaceNormal(ray, normal(intPoint));
		hitRecord.material = getMaterial();
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
