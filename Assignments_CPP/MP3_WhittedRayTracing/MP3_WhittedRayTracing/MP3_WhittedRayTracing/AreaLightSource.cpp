#include "AreaLightSource.h"

AreaLightSource::AreaLightSource(const Point3D(&v)[3], const std::shared_ptr<Material>& material) : Triangle(v, material, ObjectType::AreaLightSource) {
	//for (size_t i = 0; i < 5; i++) {
	//	samplePoints.push_back(v[0]);
	//}
	Arithmetic::generateTriangleSamplePoints(vertex0(), vertex1(), vertex2(), 25, samplePoints);
}

const std::vector<Point3D> AreaLightSource::getSamplePoints() const
{
	return samplePoints;
}