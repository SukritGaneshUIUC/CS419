
#include "Camera.h"

/*
* @return the position of the camera in camera space
*/
const Point3D& Camera::getPosition() const
{
	return position;
}

/*
* @return the position of the center of the view window in camera space
*/
const Point3D& Camera::getViewWindowPosition() const
{
	return viewWindowPosition;
}

/*
* @return the distance from the camera position to the view window position
*/
const double& Camera::getViewWindowDistance() const
{
	return d;
}

/*
* @return the up vector of the camera in camera space
*/
const Vec3D& Camera::getUpVector() const
{
	return upVector;
}

/*
* @return the number of rows in the view window
*/
const int& Camera::getViewWindowRows() const
{
	return viewWindowRows;
}

/*
* @return the number of columns in the view window
*/
const int& Camera::getViewWindowCols() const
{
	return viewWindowCols;
}

/*
* @return the size of the square pixels in camera space
*/
const double& Camera::getPixelSize() const
{
	return pixelSize;
}

/*
* @return the type of projection - ORTHOGRAPHIC or PERSPECTIVE
*/
const ProjectionType& Camera::getProjectionType() const
{
	return projectionType;
}

/*
* @return the position of the camera in world space
*/
const Point3D& Camera::getWorldPosition() const
{
	return worldPosition;
}

/*
* @param the position of the camera in camera space
*/
void Camera::setPosition(const Point3D& position)
{
	this->position = position;
}

/*
* @param the view window position in camera space
*/
void Camera::setViewWindowPosition(const Point3D& viewWindowPosition)
{
	this->viewWindowPosition = viewWindowPosition;
}

/*
* @param the distance from the camera position to the view window position
*/
void Camera::setViewWindowDistance(const double& viewWindowDistance)
{
	this->d = viewWindowDistance;
}

/*
* @param the up vector in camera space
*/
void Camera::setUpVector(const Vec3D& upVector)
{
	this->upVector = upVector.get_normalized();
}

/*
* @param the number of view window rows
*/
void Camera::setViewWindowRows(const int& viewWindowRows)
{
	this->viewWindowRows = viewWindowRows;
}

/*
* @param the number of view window columns
*/
void Camera::setViewWindowCols(const int& viewWindowCols)
{
	this->viewWindowCols = viewWindowCols;
}

/*
* @param the size of pixels
*/
void Camera::setPixelSize(const double& pixelSize)
{
	this->pixelSize = pixelSize;
}

/*
* @param the type of projection
*/
void Camera::setProjectionType(const ProjectionType& projectionType)
{
	this->projectionType = projectionType;
}

/*
* @param worldPosition The position of the camera in world coordinates
*/
void Camera::setWorldPosition(const Point3D& worldPosition)
{
	this->worldPosition = worldPosition;
}

/*
* calculate the w, u, and v vectors, along with the view window distance d
* essential step before performing ray tracing
*/
void Camera::ready()
{
	w = position - viewWindowPosition;
	d = w.euclidean();
	w.normalize();
	u = (upVector.crossProduct(w)).get_normalized();
	v = w.crossProduct(u);
	//std::cout << "pos: " << position.toString() << " - " << viewWindowPosition.toString() << "=" << (position - viewWindowPosition).toString() << std::endl;
	//std::cout << "whee2: " << upVector.toString() << " x " << w.toString() << " = " << upVector.crossProduct(w).toString() << std::endl;
}

/*
* Converts a point from view to camera space
* 
* @param point The point in view space
* 
* @return The point in world space
*/
Point3D Camera::toWorldSpace(const Point3D& point)
{
	return (u * point.x()) + (v * point.y()) + (w * point.z());
}

/*
* perform ray-tracing given a row and column number
* top-right of view window corresponds to (0,0)
* 
* @param row The row to send the ray through
* @param column The column to send the ray through
* @param xOffset The horizontal offset of the ray destination
* @param yOffset The vertical offset of the ray destination
* @param start The origin of the ray, set by function
* @param direction The direction of the ray, set by function
*
*/
bool Camera::getRay(const int& row, const int& column, const double& xOffset, const double& yOffset, Point3D& start, Vec3D& direction)
{
	// calculate destination of ray in view space
	double xv = pixelSize * (column - viewWindowCols / 2 + xOffset);
	double yv = -1.0 * pixelSize * (row - viewWindowRows / 2 + yOffset);	// NOTE: (0,0) corresponds to pixel at top-right corner
	double zv = -1 * d;
	Point3D destination_v(xv, yv, zv);

	// calculate start of ray in view space
	// ray start remains constant with perspective projection, changes with orthographic projection (mirror of view window at viewposition)
	Point3D start_v = ORIGIN;
	if (projectionType == ProjectionType::ORTHOGRAPHIC) {
		start_v[0] += xv;
		start_v[1] += yv;
	}

	// apply transformations
	Point3D start_w = toWorldSpace(start_v);
	Point3D destination_w = toWorldSpace(destination_v);
	
	// construct ray (based on transformed points)
	start = start_w;
	direction = destination_w - start_w;

	return true;
}

//bool Camera::getRay(const int& row, const int& column, const double& xOffset, const double& yOffset, Point3D& start, Vec3D& direction)
//{
//	double xv = pixelSize * (column - viewWindowCols / 2 + xOffset);
//	double yv = -1.0 * pixelSize * (row - viewWindowRows / 2 + yOffset);	// NOTE: (0,0) corresponds to pixel at top-right corner
//	double zv = -1 * d;
//
//	// create currWorldPosition depending on perspective or orthographic projection
//	// world position remains constant with perspective projection, changes with orthographic projection (mirror of view window at viewposition)
//	start = worldPosition;
//	if (projectionType == ProjectionType::ORTHOGRAPHIC) {
//		start[0] = worldPosition.x() + xv;
//		start[1] = worldPosition.y() + yv;
//		start[2] = worldPosition.z();
//	}
//
//	Vec3D dv{ Vec3D(xv, yv, zv) - start };
//	//std::cout << u.toString() << " | " << v.toString() << " | " << w.toString() << " | " << dv.toString() << std::endl;
//	direction = (u * dv[0]) + (v * dv[1]) + (w * dv[2]);
//
//	return true;
//}

