#include "Vec3D.h"

/*
* Cap all elements of the Vec3D at a value
* 
* @param maxVal The maximum cap value
*/
void Vec3D::capValues(const double& maxVal)
{
	for (int i = 0; i < 3; i++) {
		pts[i] = std::min(pts[i], maxVal);
	}
}

/*
* Default constructor
*/
Vec3D::Vec3D() : pts{ 0,0,0 } {}

/*
* Constructor
* 
* @param x The first element
* @param y The second element
* @param z The third element
*/
Vec3D::Vec3D(double x, double y, double z) : pts{ x,y,z } {}

/*
* Constructor
* 
* @param arr The elements
*/
Vec3D::Vec3D(const double(&arr)[3]) : pts{ arr[0], arr[1], arr[2] } {}

/*
* Copy constructor
* 
* @param other The Vec3D to be copied
*/
Vec3D::Vec3D(const Vec3D& other): pts{other.x(), other.y(), other.z()} {}

/*
* Copy assignment operator
*
* @param other The Vec3D to be copied
*/
void Vec3D::operator=(const Vec3D& other)
{
	if (this != &other) {
		pts[0] = other.pts[0];
		pts[1] = other.pts[1];
		pts[2] = other.pts[2];
	}
}

/*
* @return The first element of the Vec3D
*/
double Vec3D::x() const { return pts[0]; }

/*
* @return The second element of the Vec3D
*/
double Vec3D::y() const { return pts[1]; }

/*
* @return The third element of the Vec3D
*/
double Vec3D::z() const { return pts[2]; }

/*
* Set the contents of the Vec3D equal to the contents of another Vec3D
* 
* @param other The Vec3D to be copied
*/
void Vec3D::setPoint(const Vec3D& other)
{
	setPoint(other.pts);
}

/*
* Set the contents of the Vec3D equal to the contents of an array
* 
* @param arr The array containing the contents
*/
void Vec3D::setPoint(const double(&arr)[3])
{
	pts[0] = arr[0];
	pts[1] = arr[1];
	pts[2] = arr[2];
}

/*
* Accessor overloaded operator
* 
* @param idx The index of the element
* 
* @return the value at idx
*/
double Vec3D::operator[](const int& idx) const
{
	return pts[idx];
}

/*
* Accessor overloaded operator
*
* @param idx The index of the element
*
* @return the value at idx
*/
double& Vec3D::operator[](const int& idx)
{
	return pts[idx];
}

/*
* Sum-Equals overloaded operator
*
* @param other The other Vec3D
*
* @return Reference to this Vec3D after operation
*/
Vec3D& Vec3D::operator+=(const Vec3D& other)
{
	pts[0] += other.pts[0];
	pts[1] += other.pts[1];
	pts[2] += other.pts[2];
	return *this;
}

/*
* Minus-Equals overloaded operator
*
* @param other The other Vec3D
*
* @return Reference to this Vec3D after operation
*/
Vec3D& Vec3D::operator-=(const Vec3D& other)
{
	pts[0] -= other.pts[0];
	pts[1] -= other.pts[1];
	pts[2] -= other.pts[2];
	return *this;
}

/*
* Times-Equals overloaded operator
*
* @param scalar The quantity by which to multiply all elements of Vec3D
*
* @return Reference to this Vec3D after operation
*/
Vec3D& Vec3D::operator*=(const double& scalar)
{
	pts[0] *= scalar;
	pts[1] *= scalar;
	pts[2] *= scalar;
	return *this;
}

/*
* Divide-Equals overloaded operator
*
* @param scalar The quantity by which to divide all elements of Vec3D
*
* @return Reference to this Vec3D after operation
*/
Vec3D& Vec3D::operator/=(const double& scalar)
{
	pts[0] /= scalar;
	pts[1] /= scalar;
	pts[2] /= scalar;
	return *this;
}

/*
* Exponent-Equals overloaded operator
*
* @param scalar The quantity by which to power all elements of Vec3D
*
* @return Reference to this Vec3D after operation
*/
Vec3D& Vec3D::operator^=(const double& exponent)
{
	pts[0] = pow(pts[0], exponent);
	pts[1] = pow(pts[1], exponent);
	pts[2] = pow(pts[2], exponent);
	return *this;
}

/*
* Sum overloaded operator
*
* @param other The other Vec3D
*
* @return Element-wise sum of this Vec3D with other
*/
Vec3D Vec3D::operator+(const Vec3D& other) const
{
	Vec3D x{ *this };
	x += other;
	return x;
}

/*
* Subtraction overloaded operator
*
* @param other The other Vec3D
*
* @return Element-wise subtraction of this Vec3D with other
*/
Vec3D Vec3D::operator-(const Vec3D& other) const
{
	Vec3D x{ *this };
	x -= other;
	return x;
}

/*
* Multiply overloaded operator
*
* @param scalar The quantity by which to multiply all elements of Vec3D
*
* @return New Vec3D equivalant to this Vec3D multiplied by scalar
*/
Vec3D Vec3D::operator*(const double& scalar) const
{
	Vec3D x{ *this };
	x *= scalar;
	return x;
}

/*
* Divide overloaded operator
*
* @param scalar The quantity by which to divide all elements of Vec3D
*
* @return New Vec3D equivalant to this Vec3D divided by scalar
*/
Vec3D Vec3D::operator/(const double& scalar) const
{
	Vec3D x{ *this };
	x /= scalar;
	return x;
}

/*
* Power overloaded operator
*
* @param exponent The quantity by which to power all elements of Vec3D
*
* @return New Vec3D equivalant to this Vec3D raised to exponent
*/
Vec3D Vec3D::operator^(const double& exponent) const
{
	Vec3D x{ *this };
	x ^= exponent;
	return x;
}

/*
* Cap values of Vec3D
* 
* @param arr The cap values of the elements
*/
void Vec3D::capValuesMax(const double(&arr)[3])
{
	for (int i = 0; i < 3; i++) {
		if (pts[i] > arr[i]) {
			pts[i] = arr[i];
		}
	}
}

/*
* Floor values of Vec3D
*
* @param arr The floor values of the elements
*/
void Vec3D::capValuesMin(const double(&arr)[3])
{
	for (int i = 0; i < 3; i++) {
		if (pts[i] < arr[i]) {
			pts[i] = arr[i];
		}
	}
}

/*
* Perform element-wise multiplication on this Vec3D with another Vec3D
* 
* @param other The other Vec3D
* 
* @return A new Vec3D formed by element-wise multiplication of this Vec3D with other
*/
Vec3D Vec3D::elementMultiply(const Vec3D& other) const
{
	return Vec3D(x() * other.x(), y() * other.y(), z() * other.z());
}

/*
* Take the dot product of this Vec3D with another Vec3D
* 
* @param other The other Vec3D
* 
* @return The dot product of this Vec3D with other
*/
double Vec3D::dotProduct(const Vec3D& other) const
{
	return pts[0] * other.pts[0] + pts[1] * other.pts[1] + pts[2] * other.pts[2];
}

/*
* Take the cross product of this Vec3D with another Vec3D
*
* @param other The other Vec3D
*
* @return The cross product of this Vec3D with other
*/
Vec3D Vec3D::crossProduct(const Vec3D& other) const
{
	const Vec3D& a = *this;
	const Vec3D& b = other;
	// <a2b3 - a3b2, a3b1 - a1b3, a1b2 - a2b1>
	return Vec3D{a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(), a.x() * b.y() - a.y() * b.x()};
}

/*
* Take the squared norm of this Vec3D
*
* @param l The norm factor
*
* @return The squared l-norm of this Vec3D
*/
double Vec3D::normSquared(const double& l) const
{
	return pow(pts[0], l) + pow(pts[1], l) + pow(pts[2], l);
}

/*
* Take the norm of this Vec3D
*
* @param l The norm factor
*
* @return The l-norm of this Vec3D
*/
double Vec3D::norm(const double& l) const
{
	return pow(normSquared(l), 1.0 / l);
}

/*
* Take the 1-norm (manhattan norm) of this Vec3D
*
* @return The 1-norm of this Vec3D
*/
double Vec3D::manhattan() const
{
	return norm(1);
}

/*
* Take the 2-norm (euclidean norm) of this Vec3D
*
* @return The 2-norm of this Vec3D
*/
double Vec3D::euclidean() const
{
	return norm(2);
}

/*
* Get the magnitude (euclidean norm) of this Vec3D
*
* @return The 2-norm of this Vec3D
*/
double Vec3D::magnitude() const
{
	return euclidean();
}

/*
* Get the squared euclidean norm of this Vec3D
*
* @return The squared 2-norm of this Vec3D
*/
double Vec3D::euclideanSquared() const
{
	return normSquared(2);
}

/*
* Get the squared magnitude (euclidean norm) of this Vec3D
*
* @return The squared 2-norm of this Vec3D
*/
double Vec3D::magnitudeSquared() const
{
	return euclideanSquared();
}

/*
* Find the Minkowski distance between this Vec3D and another Vec3D
* Default l-value is 2
*
* @return The l-Minkowski distance between this Vec3D and other
*/
double Vec3D::distanceTo(const Vec3D& other, const int& dist_l) const
{
	return (*this - other).normSquared(dist_l);
}

/*
* Return the normalized form of this Vec3D
* 
* @param l The l-value of the norm
* 
* @return The normalized form of this Vec3D
*/
Vec3D Vec3D::get_normalized(double l) const
{
	Vec3D newVec{ *this };
	newVec.normalize(l);
	return newVec;
}

/*
* Normalize this Vec3D
* 
* @param l The l-value of the norm
*/
void Vec3D::normalize(double l)
{
	const double factor = norm(l);
	pts[0] /= factor;
	pts[1] /= factor;
	pts[2] /= factor;
}

/*
* Change the brightness of this ColorRGB
* Values capped at RGB_MAX
* 
* @param factor The factor by which to raise brightness
*/
void Vec3D::changeBrightness(const double& factor)
{
	(*this) *= factor;
	capValues(RGB_MAX);
}

/*
* Format Vec3D into a user-friendly string
* 
* @return The string formed by formatting this Vec3D
*/
std::string Vec3D::toString() const
{
	std::stringstream ss;
	ss << x() << " " << y() << " " << z();
	return ss.str();
}

/*
* Format Vec3D into a PPM string
* Integer values separated by space
*
* @return The string formed by formatting this Vec3D to PPM specifications
*/
std::string Vec3D::renderString() const
{
	std::stringstream ss;
	ss << (int) x() << " " << (int) y() << " " << (int) z();
	return ss.str();
}
