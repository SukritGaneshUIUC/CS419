#pragma once

#include "Vec3D.h"
#include "Ray3D.h"
#include <cmath>

constexpr int AFFINE_DIMS = 4;
constexpr int AFFINE_ENTRIES = AFFINE_DIMS * AFFINE_DIMS;

enum class Transformation { SCALE, ROTATE, TRANSLATE };

class Mat4
{
private:

	double elements[AFFINE_DIMS * AFFINE_DIMS];

	void copy(const Mat4& other);

public:
	Mat4();
	Mat4(const Mat4& other);

	const double& get(const size_t& row, const size_t& col) const;
	void set(const size_t& row, const size_t& col, const double& newVal);
	void getRow(const size_t& row, double(&arr)[AFFINE_DIMS]) const;
	void getCol(const size_t& col, double(&arr)[AFFINE_DIMS]) const;
	double dotProduct(const double(&v1)[AFFINE_DIMS], const double(&v2)[AFFINE_DIMS]) const;

	Mat4& operator*=(const Mat4& other);
	Mat4 operator*(const Mat4& other) const;

	Mat4 fromIdentity();
	Mat4 fromTranslation(const Vec3D& translationAmount);
	Mat4 fromRotation(const double& rotationAmount, const Vec3D& axis);
	Mat4 fromScale(const Vec3D& scaleAmount);

	void translate(const Vec3D& translationAmount);
	void rotate(const double& rotationAmount, const Vec3D& axis);
	void scale(const Vec3D& scaleAmount);

	Mat4 getTranspose() const;
	Mat4 getSingleTransformationInverse(const Transformation& transformationType) const;
};

