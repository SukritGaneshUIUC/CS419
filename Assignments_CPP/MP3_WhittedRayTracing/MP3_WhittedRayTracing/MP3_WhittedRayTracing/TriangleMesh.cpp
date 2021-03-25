#include "TriangleMesh.h"

/*
* Constructor for TriangleMesh. Initializes matrices to identity matrix, colors to pink.
*/
TriangleMesh::TriangleMesh() : modelViewMatrix(Mat4()), inverseModelViewMatrix(Mat4()), material(Material(PINK_COLOR, PINK_COLOR, WHITE_COLOR, DEFAULT_ALPHA)) {}

/*
* Sets the mesh's material.
*
* @param material The Material of the TriangleMesh
*/
void TriangleMesh::setMaterial(const Material& material)
{
	this->material = material;
}

/*
* Load a model from an OBJ File.
*
* @param path The filepath of the OBJ file
*/
void TriangleMesh::loadFromOBJFile(const std::string& path)
{
	std::ifstream file(path);
	std::string line;

	// Read in vertices and faces
	while (std::getline(file, line)) {

		//std::cout << "Line: |" << line << "|" << std::endl;

		if (line[0] == '#') {
			continue;
		}
		else if (line[0] == 'v') {
			std::string lf;
			double x, y, z;
			std::istringstream iss{ line };
			if (!(iss >> lf >> x >> y >> z)) {
				break;
			}
			std::shared_ptr<Point3D> p{ new Point3D{x, y, z} };
			vertices.push_back(p);
		}
		else if (line[0] == 'f') {
			std::string lf;
			size_t v0, v1, v2;
			std::istringstream iss{ line };
			if (!(iss >> lf >> v0 >> v1 >> v2)) {
				break;
			}
			std::shared_ptr<TriangleFace> f{ new TriangleFace{ v0 - 1, v1 - 1, v2 - 1 } };
			faces.push_back(f);
		}
	}

	// Compute Vertex Normals
	std::cout << "faces: " << faces.size() << std::endl;
	computeNormals(vertices, faces, normals);

	// Generate Array of Triangles
	generateTriangles(vertices, faces, normals, triangles);
}

/*
* Add a transformation from the model space to the world space.
*
* @param transformationMatrix The transformation matrix
* @param transformationType The type of transformation being performed (ROTATE, SCALE, TRANSLATE)
*/
void TriangleMesh::addTransformation(const Mat4& transformationMatrix, const Transformation& transformationType)
{
	modelViewMatrix = transformationMatrix * modelViewMatrix;
	Mat4 inverseTransformationMatrix{ transformationMatrix.getSingleTransformationInverse(transformationType) };
	inverseModelViewMatrix = inverseModelViewMatrix * inverseTransformationMatrix;
}

/*
* Compute the vertex normal vectors of a given face-set
*
* @param vertices The vertices
* @param faces The faces (each face contains three vertex indices)
* @param normals Vector which will contain vertex normals. Will be changed by function.
*/
void TriangleMesh::computeNormals(const std::vector<std::shared_ptr<Point3D>>& vertices, const std::vector<std::shared_ptr<TriangleFace>>& faces, std::vector<std::shared_ptr<Vec3D>>& normals)
{
	// MP2: Implement this function!

	for (size_t i = 0; i < vertices.size(); i++) {
		normals.push_back(std::make_shared<Vec3D>(0, 0, 0));
	}

	// Iterate over all triangles
	for (size_t i = 0; i < faces.size(); i++) {

		size_t v0_idx = faces[i]->v0_idx;
		size_t v1_idx = faces[i]->v1_idx;
		size_t v2_idx = faces[i]->v2_idx;

		const std::shared_ptr<Point3D>& v0 = vertices[v0_idx];
		const std::shared_ptr<Point3D>& v1 = vertices[v1_idx];
		const std::shared_ptr<Point3D>& v2 = vertices[v2_idx];

		// Find the normal vector of the current triangle
		Vec3D vec1{ (*v1) - (*v0) };
		Vec3D vec2{ (*v2) - (*v0) };
		Vec3D currNormal{ vec2.crossProduct(vec1) };
		Vec3D scaledNormal = currNormal * (0.5 * currNormal.magnitude());

		// Add the normal vector to the cumulative normals of v0, v1, and v2
		normals[v0_idx]->operator+=(scaledNormal);
		normals[v1_idx]->operator+=(scaledNormal);
		normals[v2_idx]->operator+=(scaledNormal);
	}

	// Normalize all the normals
	for (size_t i = 0; i < normals.size(); i++) {
		normals[i]->normalize();
		//std::cout << "VNorm: " << normals[i]->toString() << std::endl;
	}
}

/*
* Compute the vertex normal vectors of a given face-set
*
* @param vertices The vertices
* @param faces The faces (each face contains three vertex indices)
* @param normals Vector which will contain vertex normals.
* @param triangles The vector of Triangles which will be generated from the given data. Modified by function.
*/
void TriangleMesh::generateTriangles(const std::vector<std::shared_ptr<Point3D>>& vertices, const std::vector<std::shared_ptr<TriangleFace>>& faces, const std::vector<std::shared_ptr<Vec3D>>& normals, std::vector<std::shared_ptr<Triangle>>& triangles)
{
	for (std::shared_ptr<TriangleFace> face : faces) {
		// Get vertices
		Point3D currVertices[3]{ *vertices[face->v0_idx], *vertices[face->v1_idx], *vertices[face->v2_idx] };
		Vec3D currNormals[3]{ *normals[face->v0_idx], *normals[face->v1_idx], *normals[face->v2_idx] };

		std::shared_ptr<Triangle> currTriangle{ new Triangle{currVertices, currNormals, material} };
		triangles.push_back(currTriangle);
	}
}

/*
* @return The vertices
*/
const std::vector<std::shared_ptr<Point3D>>& TriangleMesh::getVertices() const
{
	return vertices;
}

/*
* @return The faces
*/
const std::vector<std::shared_ptr<TriangleFace>>& TriangleMesh::getFaces() const
{
	return faces;
}

/*
* @return The normals
*/
const std::vector<std::shared_ptr<Vec3D>>& TriangleMesh::getNormals() const
{
	return normals;
}

/*
* @return The triangles
*/
const std::vector<std::shared_ptr<Triangle>> TriangleMesh::getTriangles() const
{
	return triangles;
}

/*
* @return The transformation matrix from model to world coordinates
*/
const Mat4& TriangleMesh::getModelViewMatrix() const
{
	return modelViewMatrix;
}

/*
* @return The transformation matrix from world to model coordinates (inverse of modelViewMatrix)
*/
const Mat4& TriangleMesh::getInverseModelViewMatrix() const
{
	return inverseModelViewMatrix;
}
