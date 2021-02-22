#pragma once

#include "Vec3D.h"

class Ray3D
{
private:
	Point3D start;
	Vec3D direction;
public:
	Ray3D();
	Ray3D(const Point3D& s, const Vec3D& d);
	Ray3D(const double(&s)[3], const double(&d)[3]);
	Ray3D(const Ray3D& other);
	~Ray3D();

	const Point3D& getStart() const;
	const Vec3D& getDirection() const;

	void setStart(const Point3D& s);
	void setStart(const double(&s)[3]);
	void setDirection(const Vec3D& d);
	void setDirection(const double(&d)[3]);
	void set(const Point3D& s, const Vec3D& d);
	void set(const double(&s)[3], const double(&d)[3]);

	Point3D pos(const double& t) const;

	std::string toString();
};

