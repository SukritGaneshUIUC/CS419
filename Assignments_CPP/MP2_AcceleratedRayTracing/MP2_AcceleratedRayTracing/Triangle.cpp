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
	
	Vec3D A = vertices[1] - vertices[0];
	Vec3D B = vertices[2] - vertices[0];
	normalVector = B.crossProduct(A);
}

/*
* @return The vertices of the Triangle
*/
const Point3D* Triangle::getVertices() const
{
	return vertices;
}

const Point3D& Triangle::operator[](const int& idx) const
{
	return vertices[idx];
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

void Triangle::setNormal(const Vec3D& normalVector)
{
	this->normalVector = normalVector;
}

///*
//* Find the intersection points, if any, with a Ray3D
//*
//* @param ray The potentially intersecting
//* @param intPoints A vector of type Point3D, to which the function will push back any intersection points
//*
//* @return The number of intersection points
//*/
//int Triangle::intersection(const Ray3D& ray, const double& t_min, const double& t_max, std::vector<double>& intTs) const
//{
//	double intT;
//	Point3D intPoint;
//	bool intersected = Arithmetic::moller_trumbore(ray.getStart(), ray.getDirection(), vertices, intT, intPoint);
//	if (intersected) {
//		intTs.push_back(intT);
//		return 1;
//	}
//	return 0;
//
//	//double t;
//	//double u;
//	//double v;
//	//bool intersected = Arithmetic::ray_intersect_triangle(ray.getStart(), ray.getDirection().get_normalized(2), vertices[0], vertices[1], vertices[2], t, u, v);
//	//if (intersected) {
//	//	intPoints.push_back(ray.pos(t));
//	//	return 1;
//	//}
//	//return 0;
//}

/*
* Find the intersection points, if any, with a Ray3D
*
* @param ray The potentially intersecting
* @param intPoints A vector of type Point3D, to which the function will push back any intersection points
*
* @return The number of intersection points
*/
int Triangle::intersection(const Ray3D& ray, const double& t_min, const double& t_max, std::vector<Point3D>& intPoints) const
{
	double intT;
	Point3D intPoint;
	bool intersected = Arithmetic::moller_trumbore(ray.getStart(), ray.getDirection(), vertices, intT, intPoint);
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
	return normalVector;
}

bool Triangle::generateBoundingBox(AABB3D& bb) const
{
	Point3D minPoint;
	Point3D maxPoint;

	for (int i = 0; i < 3; i++) {
		minPoint[i] = std::min(vertices[0][i], std::min(vertices[1][i], vertices[2][i]));
		maxPoint[i] = std::max(vertices[0][i], std::max(vertices[1][i], vertices[2][i]));
	}

	bb.min() = minPoint;
	bb.max() = maxPoint;
	return true;
}
