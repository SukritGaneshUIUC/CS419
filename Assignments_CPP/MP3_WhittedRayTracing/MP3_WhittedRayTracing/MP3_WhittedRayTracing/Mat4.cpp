#include "Mat4.h"

/*
* Copy a Mat4
*
* @param other The Mat4 to copy.
*/
void Mat4::copy(const Mat4& other)
{
	for (size_t row = 0; row < AFFINE_DIMS; row++) {
		for (size_t col = 0; col < AFFINE_DIMS; col++) {
			this->set(col, row, other.get(row, col));
		}
	}
}

/*
* Default constructor for Mat4
* Sets elements to equal a 4x4 identity matrix
*/
Mat4::Mat4() : elements{ 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 } {}

/*
* Copy Constructor for Mat4
*
* @param other The Mat4 to copy.
*/
Mat4::Mat4(const Mat4& other)
{
	this->copy(other);
}

/*
* Get the matrix entry at a specified row and column
*
* @param row The row
* @param col The column
*
* @return The element at [row][col]
*/
const double& Mat4::get(const size_t& row, const size_t& col) const
{
	return elements[row * AFFINE_DIMS + col];
}

/*
* Set the matrix entry at a specified row and column to a specified value
*
* @param row The row
* @param col The column
* @param newVal The value to set the entry at [row][col] to
*/
void Mat4::set(const size_t& row, const size_t& col, const double& newVal)
{
	elements[row * AFFINE_DIMS + col] = newVal;
}

/*
* Returns an entire row of the matrix
*
* @param row The row index
* @param arr The array of length 4 to hold the row
*/
void Mat4::getRow(const size_t& row, double(&arr)[AFFINE_DIMS]) const
{
	for (size_t col = 0; col < AFFINE_DIMS; col++) {
		arr[col] = get(row, col);
	}
}

/*
* Returns an entire column of the matrix
*
* @param col The column index
* @param arr The array of length 4 to hold the column
*/
void Mat4::getCol(const size_t& col, double(&arr)[AFFINE_DIMS]) const
{
	for (size_t row = 0; row < AFFINE_DIMS; row++) {
		arr[row] = get(row, col);
	}
}

/*
* Returns the dor product of two arrays of length 4
*
* @param v1 The first array
* @param v2 The second array
*
* @return v1 <dot> v2
*/
double Mat4::dotProduct(const double(&v1)[AFFINE_DIMS], const double(&v2)[AFFINE_DIMS]) const
{
	double dp = 0;
	for (size_t i = 0; i < AFFINE_DIMS; i++) {
		dp += (v1[i] * v2[i]);
	}
	return dp;
}

/*
* Multiplies this with another Mat4, and stores the result in this
*
* @param other The Mat4 to multiply this by
*
* @return *this
*/
Mat4& Mat4::operator*=(const Mat4& other)
{
	Mat4 multResult{};
	double currCol2[AFFINE_DIMS];
	double currRow1[AFFINE_DIMS];

	// Perform matrix multiplication
	for (size_t col2 = 0; col2 < AFFINE_DIMS; col2++) {
		other.getCol(col2, currCol2);
		for (size_t row1 = 0; row1 < AFFINE_DIMS; row1++) {
			this->getRow(row1, currRow1);
			double entry = this->dotProduct(currRow1, currCol2);
			multResult.set(row1, col2, entry);
		}
	}

	// Copy over results into elements
	this->copy(multResult);

	// Return this
	return *this;
}

/*
* Multiplies this with another Mat4, and returns the result
*
* @param other The Mat4 to multiply this by
*
* @return *this * other
*/
Mat4 Mat4::operator*(const Mat4& other) const
{
	Mat4 temp = *this;
	temp *= other;
	return temp;
}

/*
* @return An identity Mat4
*/
Mat4 Mat4::fromIdentity()
{
	return Mat4();
}

/*
* Generates a translation Mat4 (affine)
*
* @param translationAmount The amount to translate by
*
* @return An identity matrix translated by translationAmount
*/
Mat4 Mat4::fromTranslation(const Vec3D& translationAmount)
{
	Mat4 temp;
	for (size_t i = 0; i < AFFINE_DIMS - 1; i++) {
		temp.set(i, 3, translationAmount[i]);
	}
	return temp;
}

/*
* Generates a rotation Mat4 (affine)
*
* @param rotationAmount The radians to rotate by
* @param axis The axis of rotation
*
* @return An identity matrix rotated by rotationAmount
*/
Mat4 Mat4::fromRotation(const double& rotationAmount, const Vec3D& axis)
{
	Mat4 temp;

	// Case 1: x-axis
	if (axis[0]) {
		temp.set(1, 1, cos(rotationAmount));
		temp.set(1, 2, sin(rotationAmount));
		temp.set(2, 1, -1.0 * sin(rotationAmount));
		temp.set(2, 2, cos(rotationAmount));
	}

	// Case 2: y-axis
	else if (axis[1]) {
		temp.set(0, 0, cos(rotationAmount));
		temp.set(0, 2, -1.0 * sin(rotationAmount));
		temp.set(2, 0, sin(rotationAmount));
		temp.set(2, 2, cos(rotationAmount));
	}

	// Case 3: z-axis
	else if (axis[2]) {
		temp.set(0, 0, cos(rotationAmount));
		temp.set(0, 1, -1.0 * sin(rotationAmount));
		temp.set(1, 0, sin(rotationAmount));
		temp.set(1, 1, cos(rotationAmount));
	}

	return temp;
}

/*
* Generates a scale matrix (affine)
*
* @param scaleAmount The amount to scale by
*
* @return An identity matrix scaled by scaleAmount
*/
Mat4 Mat4::fromScale(const Vec3D& scaleAmount)
{
	Mat4 temp;
	for (size_t i = 0; i < AFFINE_DIMS - 1; i++) {
		temp.set(i, i, scaleAmount[i]);
	}
	return temp;
}

/*
* Applies a translation transformation to this
*
* @param translationAmount The amount to translate by
*/
void Mat4::translate(const Vec3D& translationAmount)
{
	Mat4 translationMatrix = this->fromTranslation(translationAmount);
	(*this) = translationMatrix * (*this);
}

/*
* Applies a rotation transformation to this
*
* @param rotationAmount The amount to rotate by (radians)
* @param axis The axis of rotation
*/
void Mat4::rotate(const double& rotationAmount, const Vec3D& axis)
{
	Mat4 rotationMatrix = this->fromRotation(rotationAmount, axis);
	(*this) = rotationMatrix * (*this);
}

/*
* Applies a scale transformation to this
*
* @param scaleAmount The amount to scale by
*/
void Mat4::scale(const Vec3D& scaleAmount)
{
	Mat4 scaleMatrix = this->fromScale(scaleAmount);
	(*this) = scaleMatrix * (*this);
}

/*
* @return The transpose of this
*/
Mat4 Mat4::getTranspose() const
{
	Mat4 transpose{};

	for (size_t row = 0; row < AFFINE_DIMS; row++) {
		for (size_t col = 0; col < AFFINE_DIMS; col++) {
			transpose.set(col, row, this->get(row, col));
		}
	}

	return transpose;
}

/*
* Get the inverse of this (if this is a Mat4 containing a single Mat4)
*/
Mat4 Mat4::getSingleTransformationInverse(const Transformation& transformationType) const
{

	if (transformationType == Transformation::SCALE) {
		Mat4 inverse{ *this };
		for (size_t i = 0; i < 3; i++) {
			inverse.set(i, i, 1 / this->get(i, i));
		}
		return inverse;
	}
	else if (transformationType == Transformation::TRANSLATE) {
		Mat4 inverse{ *this };
		for (size_t row = 0; row < 3; row++) {
			inverse.set(row, 3, -1 * (this->get(row, 3)));
		}
		return inverse;
	}
	else if (transformationType == Transformation::ROTATE) {
		return this->getTranspose();
	}

	return Mat4();
}
