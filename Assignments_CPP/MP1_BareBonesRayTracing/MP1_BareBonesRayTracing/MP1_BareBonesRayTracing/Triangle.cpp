#include "Triangle.h"

/*
* Constructor for Triangle
*
* @param v The vertices of the Triangle
* @param ambient The ambient color of the Triangle
* @param diffuse The diffuse color of the Triangle
* @param specular The specular color of the Triangle
* @param alpha The alpha factor of the Triangle
*/
Triangle::Triangle(const Point3D(&v)[3], const ColorRGB& diffuse, const ColorRGB& ambient, const ColorRGB& specular, const double& alpha) : SceneObject(ObjectType::Triangle, ambient, diffuse, specular, alpha)
{
	vertices[0] = v[0];
	vertices[1] = v[1];
	vertices[2] = v[2];
}

/*
* @return The vertices of the Triangle
*/
const Point3D* Triangle::getVertices() const
{
	return vertices;
}

/*
* @return The first vertex of the Triangle
*/
const Point3D& Triangle::vertex0() const
{
	return vertices[0];
}

/*
* @return The second vertex of the Triangle
*/
const Point3D& Triangle::vertex1() const
{
	return vertices[1];
}

/*
* @return The third vertex of the Triangle
*/
const Point3D& Triangle::vertex2() const
{
	return vertices[2];
}

/*
* Find the intersection points, if any, with a Ray3D
*
* @param ray The potentially intersecting
* @param intPoints A vector of type Point3D, to which the function will push back any intersection points
*
* @return The number of intersection points
*/
int Triangle::intersection(const Ray3D& ray, std::vector<Point3D>& intPoints) const
{
	Point3D intPoint;
	bool intersected = Arithmetic::moller_trumbore(ray.getStart(), ray.getDirection(), vertices, intPoint);
	if (intersected) {
		intPoints.push_back(intPoint);
		return 1;
	}
	return 0;

	//double t;
	//double u;
	//double v;
	//bool intersected = Arithmetic::ray_intersect_triangle(ray.getStart(), ray.getDirection().get_normalized(2), vertices[0], vertices[1], vertices[2], t, u, v);
	//if (intersected) {
	//	intPoints.push_back(ray.pos(t));
	//	return 1;
	//}
	//return 0;
}

/*
* Find the normal vector at a given point (irregardless of position on the surface)
* Overridden virtual function
*
* @param intersection The point on the surface at which to find the normal vector
*
* @return The normal vector
*/
Vec3D Triangle::normal(const Point3D& intersection) const
{
	Vec3D A = vertices[1] - vertices[0];
	Vec3D B = vertices[2] - vertices[0];
	return B.crossProduct(A);
}
