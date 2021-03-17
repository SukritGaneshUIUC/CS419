#include "Sphere.h"

/*
* Constructor for Sphere
*
* @param center The center of the Sphere
* @param radius The radius of the Sphere
* @param ambient The ambient color of the Sphere
* @param diffuse The diffuse color of the Sphere
* @param specular The specular color of the Sphere
* @param alpha The alpha factor of the Sphere
*/
Sphere::Sphere(const Point3D& center, const double& radius, const ColorRGB& ambient, const ColorRGB& diffuse, const ColorRGB& specular, const double& alpha) : SceneObject(ObjectType::Sphere, ambient, diffuse, specular, alpha), center(center), radius(radius) {}

/*
* @return The center of the Sphere
*/
const Point3D& Sphere::getCenter() const
{
	return center;
}

/*
* @return The radius of the Sphere
*/
const double& Sphere::getRadius() const
{
	return radius;
}

/*
* Find the intersection points, if any, with a Ray3D
*
* @param ray The potentially intersecting
* @param intPoints A vector of type Point3D, to which the function will push back any intersection points
*
* @return The number of intersection points
*/
int Sphere::intersection(const Ray3D& ray, const double& t_min, const double& t_max, HitRecord& hitRecord) const
{
	const Point3D& A = ray.getStart();
	const Point3D& C = center;
	const Vec3D& B = ray.getDirection();
	const double& R = radius;

	double a = B.euclideanSquared();
	double b = 2.0 * B.dotProduct(A - C);
	double c = (A - C).euclideanSquared() - pow(R, 2.0);

	std::vector<double> potentialSols;
	int sol_count = Arithmetic::quadratic_solver(a, b, c, potentialSols);
	
	if (sol_count == 0) {
		return 0;
	}
	std::vector<double> sols;
	for (double ps : potentialSols) {
		if (!(ps < Arithmetic::EPSILON || ps < t_min || ps > t_max)) {
			sols.push_back(ps);
		}
	}
	if (sols.size() == 0) {
		return 0;
	}
	double intT = *(std::min_element(sols.begin(), sols.end()));
	Point3D intPoint = ray.pos(intT);
	hitRecord = HitRecord(intT, intPoint, this->normal(intPoint), getAmbient(), getDiffuse(), getSpecular(), getAlpha());
	return 1;
}

/*
* Find the normal vector at a given point
* Overridden virtual function
*
* @param intersection The point on the surface at which to find the normal vector
*
* @return The normal vector
*/
Vec3D Sphere::normal(const Point3D& intersection) const
{
	return (intersection - center).get_normalized();
}

bool Sphere::generateBoundingBox(AABB3D& bb) const
{
	Point3D minPoint(center[0] - radius, center[1] - radius, center[2] - radius);
	Point3D maxPoint(center[0] + radius, center[1] + radius, center[2] + radius);

	bb.min() = minPoint;
	bb.max() = maxPoint;
	return true;
}
