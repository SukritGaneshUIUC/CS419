#include "Triangle.h"

/*
* Constructor for Triangle
*
* @param v The vertices of the Triangle
* @param material The Material of the Triangle
*/
Triangle::Triangle(const Point3D(&v)[3], const Material& material) : Object(ObjectType::Triangle, material)
{
	vertices[0] = v[0];
	vertices[1] = v[1];
	vertices[2] = v[2];

	Vec3D A = vertices[1] - vertices[0];
	Vec3D B = vertices[2] - vertices[0];
	normals[0] = B.crossProduct(A);
	normals[0].normalize();

	meshTriangle = false;

	Point3D minPoint;
	Point3D maxPoint;

	for (int i = 0; i < 3; i++) {
		minPoint[i] = std::min(vertices[0][i], std::min(vertices[1][i], vertices[2][i]));
		maxPoint[i] = std::max(vertices[0][i], std::max(vertices[1][i], vertices[2][i]));
	}

	boundingBox = AABB3D();
	boundingBox.min() = minPoint;
	boundingBox.max() = maxPoint;
}

/*
* Constructor for Triangle (on mesh, given 3 vertex normals)
*
* @param v The vertices of the Triangle
* @param n The normals of the vertices of the Triangle (from mesh)
* @param material The Material of the Triangle
*/
Triangle::Triangle(const Point3D(&v)[3], const Vec3D(&n)[3], const Material& material) : Object(ObjectType::Triangle, material)
{
	vertices[0] = v[0];
	vertices[1] = v[1];
	vertices[2] = v[2];

	normals[0] = n[0];
	normals[1] = n[1];
	normals[2] = n[2];

	//normals[0] = n[0].get_normalized();
	//normals[1] = n[1].get_normalized();
	//normals[2] = n[2].get_normalized();

	meshTriangle = true;

	Point3D minPoint;
	Point3D maxPoint;

	for (int i = 0; i < 3; i++) {
		minPoint[i] = std::min(vertices[0][i], std::min(vertices[1][i], vertices[2][i]));
		maxPoint[i] = std::max(vertices[0][i], std::max(vertices[1][i], vertices[2][i]));
	}

	boundingBox = AABB3D();
	boundingBox.min() = minPoint;
	boundingBox.max() = maxPoint;
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
int Triangle::intersection(const Ray3D& ray, const double& t_min, const double& t_max, HitRecord& hitRecord) const
{
	double intT;
	Point3D intPoint;
	bool intersected = Arithmetic::moller_trumbore(ray.getStart(), ray.getDirection(), vertices, intT, intPoint);
	if (intersected) {
		if (intT < t_min || intT > t_max) {
			return 0;
		}
		hitRecord = HitRecord(intT, intPoint, this->normal(intPoint), getMaterial());
		return 1;
	}
	return 0;
}

/*
* Get the normal vector of the object at a specified intersection point
* May use face or barycentric normal
*
* @param intersection The intersection point
*
* @return The normal vector of the surface at intersection
*/
Vec3D Triangle::normal(const Point3D& intersection) const
{
	if (!meshTriangle) {
		return normals[0];
	}
	double u, v, w;
	Arithmetic::barycentric(intersection, vertices[0], vertices[1], vertices[2], u, v, w);
	Vec3D weightedNormal{ (normals[0] * u) + (normals[1] * v) + (normals[2] * w) };
	weightedNormal.normalize();
	//std::cout << "tn: " << weightedNormal.toString() << std::endl;
	return weightedNormal;

}

/*
* Generates an axis-aligned bounding box for the object
*
* @param bb An AABB3D to hold the axis-aligned bounding box of the object. Modified by functions.
*/
bool Triangle::generateBoundingBox(AABB3D& bb) const
{
	bb = boundingBox;
	return true;
}

/*
* @return Triangle in pretty string format
*/
std::string Triangle::toString() const
{
	return vertices[0].toString() + ", " + vertices[1].toString() + ", " + vertices[2].toString();
}
