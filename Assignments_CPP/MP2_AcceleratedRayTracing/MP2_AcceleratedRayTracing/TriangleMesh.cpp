#include "TriangleMesh.h"

/*
* Constructor for TriangleMesh. Initializes matrices to identity matrix, colors to pink.
*/
TriangleMesh::TriangleMesh() : modelViewMatrix(Mat4()), inverseModelViewMatrix(Mat4()), ambient(PINK_COLOR), diffuse(PINK_COLOR), specular(WHITE_COLOR), alpha(DEFAULT_ALPHA) {}

/*
* Sets the mesh's colors.
* 
* @param ambient The ambient color
* @param diffuse The diffuse color
* @param specular The specular color
* @param alpha The alpha factor
*/
void TriangleMesh::setColors(const ColorRGB& ambient, const ColorRGB& diffuse, const ColorRGB& specular, const double& alpha)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->alpha = alpha;
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

	while (std::getline(file, line)) {

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

	computeNormals(vertices, faces, normals);
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
		normals.push_back(std::make_shared<Vec3D>(Vec3D{ 0,0,0 }));
	}

	// Iterate over all triangles
	for (size_t i = 0; i < faces.size(); i++) {
		//size_t v0_idx = faces[i].get()->operator[](0);
		//size_t v1_idx = faces[i].get()->operator[](1);
		//size_t v2_idx = faces[i].get()->operator[](2);
		size_t v0_idx = faces[i].get()->v0_idx;
		size_t v1_idx = faces[i].get()->v1_idx;
		size_t v2_idx = faces[i].get()->v2_idx;

		const Point3D* const v0 = vertices[v0_idx].get();
		const Point3D* const v1 = vertices[v0_idx].get();
		const Point3D* const v2 = vertices[v0_idx].get();

		// Find the normal vector of the current triangle
		Vec3D vec1{ (*v1) - (*v0) };
		Vec3D vec2{ (*v2) - (*v0) };
		Vec3D currNormal{ vec1.crossProduct(vec2) };
		Vec3D scaledNormal = currNormal * (0.5 * currNormal.magnitude());

		// Add the normal vector to the cumulative normals of v0, v1, and v2
		normals[v0_idx].get()->operator+=(scaledNormal);
		normals[v1_idx].get()->operator+=(scaledNormal);
		normals[v2_idx].get()->operator+=(scaledNormal);
	}

	// Normalize all the normals
	for (size_t i = 0; i < normals.size(); i++) {
		normals[i].get()->normalize(2);
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
