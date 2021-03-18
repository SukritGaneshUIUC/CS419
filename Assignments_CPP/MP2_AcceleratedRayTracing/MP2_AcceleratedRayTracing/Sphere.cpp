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
Sphere::Sphere(const Point3D& center, const double& radius, const ColorRGB& ambient, const ColorRGB& diffuse, const ColorRGB& specular, const double& alpha) : SceneObject(ObjectType::Sphere, ambient, diffuse, specular, alpha), center(center), radius(radius) {
	Point3D minPoint(center[0] - radius, center[1] - radius, center[2] - radius);
	Point3D maxPoint(center[0] + radius, center[1] + radius, center[2] + radius);

	boundingBox = AABB3D();
	boundingBox.min() = minPoint;
	boundingBox.max() = maxPoint;
}

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
* @param ray A Ray3D.
* @param t_min The minimum t-value of the intersection.
* @param t_max The maximum t-value of the intersection.
* @param hitRecord A HitRecord struct which will store information related to the intersection (if any). Modified by function.
*
* @return The number of intersection points (1 or 0)
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
* Get the normal vector of the object at a specified intersection point
*
* @param intersection The intersection point
*
* @return The normal vector of the surface at intersection
*/
Vec3D Sphere::normal(const Point3D& intersection) const
{
	return (intersection - center).get_normalized();
}

/*
* Generates an axis-aligned bounding box for the object
* 
* @param bb An AABB3D to hold the axis-aligned bounding box of the object. Modified by functions.
*/
bool Sphere::generateBoundingBox(AABB3D& bb) const
{
	bb = boundingBox;
	return true;
}
