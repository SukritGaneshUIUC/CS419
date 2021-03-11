#pragma once

#include <vector>
#include <memory>
#include <fstream>
#include <sstream>

#include "Triangle.h"
#include "Mat4.h"

struct TriangleFace {
	size_t v0_idx;
	size_t v1_idx;
	size_t v2_idx;

	TriangleFace(const size_t& v0_idx, const size_t& v1_idx, const size_t& v2_idx) {
		this->v0_idx = v0_idx;
		this->v1_idx = v1_idx;
		this->v2_idx = v2_idx;
	}

	TriangleFace(const size_t(&v_idxs)[3]) {
		v0_idx = v_idxs[0];
		v1_idx = v_idxs[1];
		v2_idx = v_idxs[2];
	}
};

class TriangleMesh
{
private:
	std::vector<std::shared_ptr<Point3D>> vertices;
	std::vector<std::shared_ptr<TriangleFace>> faces;
	std::vector<std::shared_ptr<Vec3D>> normals;
	Mat4 modelViewMatrix;
	Mat4 inverseModelViewMatrix;

	ColorRGB ambient;
	ColorRGB diffuse;
	ColorRGB specular;
	double alpha;

public:
	TriangleMesh();
	
	void setColors(const ColorRGB& ambient, const ColorRGB& diffuse, const ColorRGB& specular, const double& alpha);
	void loadFromOBJFile(const std::string& path);
	void addTransformation(const Mat4& transformationMatrix, const Transformation& transformationType);

	void computeNormals(const std::vector<std::shared_ptr<Point3D>>& vertices, const std::vector<std::shared_ptr<TriangleFace>>& faces, std::vector<std::shared_ptr<Vec3D>>& normals);

	const std::vector<std::shared_ptr<Point3D>>& getVertices() const;
	const std::vector<std::shared_ptr<TriangleFace>>& getFaces() const;
	const std::vector<std::shared_ptr<Vec3D>>& getNormals() const;
	const Mat4& getModelViewMatrix() const;
	const Mat4& getInverseModelViewMatrix() const;
};

