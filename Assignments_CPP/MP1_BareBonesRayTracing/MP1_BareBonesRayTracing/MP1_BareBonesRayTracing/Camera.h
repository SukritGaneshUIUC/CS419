#pragma once

#include <cmath>
#include <iostream>

#include "Arithmetic.h"
#include "Vec3D.h"
#include "Ray3D.h"

enum class ProjectionType { ORTHOGRAPHIC, PERSPECTIVE };

constexpr double DEFAULT_ASPECT_RATIO{ 16.0 / 9.0 };
constexpr double DEFAULT_VIEW_WINDOW_HEIGHT = 8.0;
constexpr double DEFAULT_VIEW_WINDOW_WIDTH = DEFAULT_VIEW_WINDOW_HEIGHT * DEFAULT_ASPECT_RATIO;

const Point3D ORIGIN({ 0,0,0 });
const Point3D DEFAULT_VIEW_WINDOW_POSITION({ 0,0,-1 });
const Vec3D DEFAULT_UP_VECTOR({ 0,1,0 });
constexpr int DEFAULT_ROWS = 720;
constexpr int DEFAULT_COLS = DEFAULT_ROWS * DEFAULT_ASPECT_RATIO;
const std::pair<int, int> DEFAULT_RESOLUTION{ DEFAULT_COLS, DEFAULT_ROWS };
constexpr double DEFAULT_PIXEL_SIZE = DEFAULT_VIEW_WINDOW_HEIGHT / DEFAULT_ROWS;
constexpr ProjectionType DEFAULT_PROJECTION_TYPE = ProjectionType::PERSPECTIVE;

class Camera
{
private:
	Point3D position;
	Point3D viewWindowPosition;
	Vec3D upVector;
	int viewWindowRows;
	int viewWindowCols;
	double pixelSize;
	ProjectionType projectionType;

	//bool ready = false;
	Vec3D u;
	Vec3D v;
	Vec3D w;
	double d;
	Point3D worldPosition;

public:
	const Point3D& getPosition() const;
	const Point3D& getViewWindowPosition() const;
	const double& getViewWindowDistance() const;
	const Vec3D& getUpVector() const;
	const int& getViewWindowRows() const;
	const int& getViewWindowCols() const;
	const double& getPixelSize() const;
	const ProjectionType& getProjectionType() const;
	const Point3D& getWorldPosition() const;

	void setPosition(const Point3D& position);
	void setViewWindowPosition(const Point3D& viewWindowPosition);
	void setViewWindowDistance(const double& viewWindowDistance);
	void setUpVector(const Vec3D& upVector);
	void setViewWindowRows(const int& viewWindowRows);
	void setViewWindowCols(const int& viewWindowCols);
	void setPixelSize(const double& pixelSize);
	void setProjectionType(const ProjectionType& projectionType);
	void setWorldPosition(const Point3D& worldPosition);

	void ready();
	bool getRay(const int& row, const int& column, const double& xOffset, const double& yOffset, Point3D& start, Vec3D& direction);	// direction vector from camera position to pixel on viewWindow
	

};
