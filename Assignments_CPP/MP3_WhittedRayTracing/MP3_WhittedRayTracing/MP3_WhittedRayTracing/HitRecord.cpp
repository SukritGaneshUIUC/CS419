#include "HitRecord.h"

/*
* Default constructor for HitRecord
*/
HitRecord::HitRecord() : intersected(false), intT(0), intPoint(BLACK_COLOR), normal(BLACK_COLOR), front_face(true), lightSource(false) {}

/*
* Set the face normal of the HitRecord.
* 
* @param ray The intersecting ray.
* @param outward_normal The outward normal of the intersected object.
*/
void HitRecord::setFaceNormal(const Ray3D& ray, const Vec3D& outward_normal) {
	front_face = ray.getDirection().dotProduct(outward_normal) < 0;
	if (front_face) {
		this->normal = outward_normal;
	}
	else {
		this->normal = outward_normal * -1;
	}
}