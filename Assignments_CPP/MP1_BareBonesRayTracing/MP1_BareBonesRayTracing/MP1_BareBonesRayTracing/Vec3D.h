#pragma once

#include <cmath>
#include <string>
#include <ostream>
#include <sstream>
#include <vector>

class Vec3D
{
private:
	double pts[3];

	void capValues(const double& maxVal);
public:
	Vec3D();
	Vec3D(double x, double y, double z);
	Vec3D(const double (&arr)[3]);
	Vec3D(const Vec3D& other);
	void operator=(const Vec3D& other);

	double x() const;
	double y() const;
	double z() const;
	void setPoint(const Vec3D& other);
	void setPoint(const double(&arr)[3]);

	double operator[](const int& idx) const;
	double& operator[](const int& idx);

	// COLOR AND VECTOR METHODS

	Vec3D& operator+=(const Vec3D& other);
	Vec3D& operator-=(const Vec3D& other);
	Vec3D& operator*=(const double& scalar);
	Vec3D& operator/=(const double& scalar);
	Vec3D& operator^=(const double& exponent);
	Vec3D operator+(const Vec3D& other) const;
	Vec3D operator-(const Vec3D& other) const;
	Vec3D operator*(const double& scalar) const;
	Vec3D operator/(const double& scalar) const;
	Vec3D operator^(const double& exponent) const;

	void capValuesMax(const double(&arr)[3]);
	void capValuesMin(const double(&arr)[3]);

	Vec3D elementMultiply(const Vec3D& other) const;

	// VECTOR ONLY METHODS

	double dotProduct(const Vec3D& other) const;
	Vec3D crossProduct(const Vec3D& other) const;

	double normSquared(const double& l) const;
	double norm(const double& l) const;
	double manhattan() const;
	double euclidean() const;
	double magnitude() const;
	double euclideanSquared() const;
	double magnitudeSquared() const;
	double distanceTo(const Vec3D& other, const int& dist_l=2) const;
	Vec3D get_normalized(double l=2) const;
	void normalize(double l=2);

	// COLOR METHODS
	void changeBrightness(const double& factor);
	
	std::string toString() const;
	std::string renderString() const;
};

using Point3D = Vec3D;
using ColorRGB = Vec3D;

constexpr int RGB_MAX = 255;
constexpr double RGB_MIN = 0.1;
const ColorRGB WHITE_COLOR({ RGB_MAX,RGB_MAX,RGB_MAX });
const ColorRGB BLACK_COLOR({ RGB_MIN,RGB_MIN,RGB_MIN });
const ColorRGB RED_COLOR({ RGB_MAX,RGB_MIN,RGB_MIN });
const ColorRGB GREEN_COLOR({ RGB_MIN,RGB_MAX,0 });
const ColorRGB BLUE_COLOR({ RGB_MIN,RGB_MIN,RGB_MAX });
const ColorRGB YELLOW_COLOR({ RGB_MAX,RGB_MAX,RGB_MIN });
const ColorRGB ORANGE_COLOR({ RGB_MAX, 165, RGB_MIN });
const ColorRGB PINK_COLOR({ RGB_MAX, 192, 203 });

