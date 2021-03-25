#include "Ray3D.h"

/*
* Default constructor for Ray3D
*/
Ray3D::Ray3D() {}

/*
* Constructor for Ray3D
*
* @param s The origin of the Ray3D
* @param d The direction vector of the Ray3D
*/
Ray3D::Ray3D(const Point3D& s, const Vec3D& d) : start(s), direction(d) {
	direction.normalize();
}

/*
* Constructor for Ray3D
*
* @param s The origin of the Ray3D
* @param d The direction vector of the Ray3D
*/
Ray3D::Ray3D(const double(&s)[3], const double(&d)[3]) : start(Point3D{ s }), direction(Vec3D{ d }) {
	direction.normalize();
}

/*
* Copy Constructor for Ray3D
*
* @param other The Ray3D to be copied
*/
Ray3D::Ray3D(const Ray3D& other) : start(other.start), direction(other.direction) {}

/*
* Destructor for Ray3D
*/
Ray3D::~Ray3D() {}

/*
* @return The origin of the Ray3D
*/
const Point3D& Ray3D::getStart() const
{
	return start;
}

/*
* @return The direction vector of the Ray3D
*/
const Vec3D& Ray3D::getDirection() const
{
	return direction;
}

/*
* @param s The origin of the Ray3D
*/
void Ray3D::setStart(const Point3D& s)
{
	start = s;
}

/*
* @param s The origin of the Ray3D
*/
void Ray3D::setStart(const double(&s)[3])
{
	start = Point3D{ s };
}

/*
* @param d The direction vector of the Ray3D
*/
void Ray3D::setDirection(const Vec3D& d)
{
	direction = d;
}

/*
* @param d The direction vector of the Ray3D
*/
void Ray3D::setDirection(const double(&d)[3])
{
	direction = Vec3D{ direction };
}

/*
* @param s The origin of the Ray3D
* @param d The direction vector of the Ray3D
*/
void Ray3D::set(const Point3D& s, const Vec3D& d)
{
	setStart(s);
	setDirection(d);
}

/*
* @param s The origin of the Ray3D
* @param d The direction vector of the Ray3D
*/
void Ray3D::set(const double(&s)[3], const double(&d)[3])
{
	setStart(s);
	setDirection(d);
}

/*
* Get the point on the ray using the formula: start + direction * t
*
* @param t The number of units of the direction vector along the Ray3D
*
* @return The point on the ray after t units of the direction vector
*/
Point3D Ray3D::pos(const double& t) const
{
	return start + direction * t;
}

/*
* Get how many "ts" along the ray the point is using the formula: t = (pos - start) / direction
*
* @param pos The point on the ray after t units of the direction vector
*
* @return The number of units of the direction vector along the Ray3D
*/
double Ray3D::getT(const Point3D& pos) const
{
	Vec3D dFull = pos - start;
	double scale = 0;
	for (int i = 0; i < 3; i++) {
		scale += dFull[i] / direction[i];
	}
	return scale / 3.0;
}

/*
* Format the Ray3D into a user-friendly string
*
* @return The string containing the contents of the Ray3D
*/
std::string Ray3D::toString()
{
	std::stringstream ss;
	ss << start.toString() << " + " << direction.toString() << "t";
	return ss.str();
}
