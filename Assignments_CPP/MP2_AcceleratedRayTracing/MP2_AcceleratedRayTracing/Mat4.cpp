#include "Mat4.h"

void Mat4::copy(const Mat4& other)
{
	for (size_t row = 0; row < AFFINE_DIMS; row++) {
		for (size_t col = 0; col < AFFINE_DIMS; col++) {
			this->set(col, row, other.get(row, col));
		}
	}
}

Mat4::Mat4() : elements{ 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 } {}

Mat4::Mat4(const Mat4& other)
{
	this->copy(other);
}

const double& Mat4::get(const size_t& row, const size_t& col) const
{
	return elements[row * AFFINE_DIMS + col];
}

void Mat4::set(const size_t& row, const size_t& col, const double& newVal)
{
	elements[row * AFFINE_DIMS + col] = newVal;
}

void Mat4::getRow(const size_t& row, double(&arr)[AFFINE_DIMS]) const
{
	for (size_t col = 0; col < AFFINE_DIMS; col++) {
		arr[col] = get(row, col);
	 }
}

void Mat4::getCol(const size_t& col, double(&arr)[AFFINE_DIMS]) const
{
	for (size_t row = 0; row < AFFINE_DIMS; row++) {
		arr[row] = get(row, col);
	}
}

double Mat4::dotProduct(const double(&v1)[AFFINE_DIMS], const double(&v2)[AFFINE_DIMS]) const
{
	double dp = 0;
	for (size_t i = 0; i < AFFINE_DIMS; i++) {
		dp += (v1[i] * v2[i]);
	}
	return dp;
}

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

Mat4 Mat4::operator*(const Mat4& other) const
{
	Mat4 temp = *this;
	temp *= other;
	return temp;
}

Mat4 Mat4::fromIdentity()
{
	return Mat4();
}

Mat4 Mat4::fromTranslation(const Vec3D& translationAmount)
{
	Mat4 temp;
	for (size_t i = 0; i < AFFINE_DIMS - 1; i++) {
		temp.set(i, 3, translationAmount[i]);
	}
	return temp;
}

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

Mat4 Mat4::fromScale(const Vec3D& scaleAmount)
{
	Mat4 temp;
	for (size_t i = 0; i < AFFINE_DIMS - 1; i++) {
		temp.set(i, i, scaleAmount[i]);
	}
	return temp;
}

void Mat4::translate(const Vec3D& translationAmount)
{
	Mat4 translationMatrix = this->fromTranslation(translationAmount);
	(*this) = translationMatrix * (*this);
}

void Mat4::rotate(const double& rotationAmount, const Vec3D& axis)
{
	Mat4 rotationMatrix = this->fromRotation(rotationAmount, axis);
	(*this) = rotationMatrix * (*this);
}

void Mat4::scale(const Vec3D& scaleAmount)
{
	Mat4 scaleMatrix = this->fromScale(scaleAmount);
	(*this) = scaleMatrix * (*this);
}

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
