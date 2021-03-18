#include "World.h"
#include "BVHNode.h"

/*
* Default constructor for World
*/
World::World() {}

/*
* Default destructor for World
*/
World::~World() {}

/*
* @return A reference to the vector of all SceneObject in the World
*/
const std::vector<std::shared_ptr<SceneObject>>& World::getSceneObjects() const
{
	return sceneObjects;
}

/*
* @return A reference to the vector of all LightSource in the World
*/
const std::vector<std::shared_ptr<LightSource>>& World::getLightSource() const
{
	return lightSources;
}

/*
* @return A reference to the triangleMesh
*/
const std::shared_ptr<TriangleMesh>& World::getTriangleMesh() const
{
	return triangleMesh;
}


/*
* @return A reference to the vector of all RenderOption in the World
*/
const std::vector<RenderOption>& World::getRenderOptions() const
{
	return renderOptions;
}

/*
* @param sceneObject The SceneObject to add to the world
*/
void World::addSceneObject(std::shared_ptr<SceneObject> sceneObject)
{

	sceneObjects.push_back(sceneObject);
}

/*
* @param lightSource The LightSource to add to the world
*/
void World::addLightSource(std::shared_ptr<LightSource> lightSource)
{
	lightSources.push_back(lightSource);
}

/*
* @param triangleMesh The TriangleMesh to add to the world
*/
void World::setTriangleMesh(const std::shared_ptr<TriangleMesh>& triangleMesh)
{
	this->triangleMesh = triangleMesh;
}

/*
* @param renderOption The RenderOption to add to the world
*/
void World::addRenderOption(const RenderOption& renderOption)
{
	renderOptions.push_back(renderOption);
}

/*
* @param image The World's Background Image
*/
void World::setBackgroundImage(Image&& image)
{
	assert(image.getRows() >= camera.getViewWindowRows());
	assert(image.getCols() >= camera.getViewWindowCols());
	backgroundImage = image;
}

/*
* @param camera The World's Camera
*/
void World::setCamera(const Camera& camera)
{
	this->camera = camera;
}

/*
* @param ambientColor The ambient light of the World
*/
void World::setAmbientLight(const ColorRGB& ambientColor)
{
	this->ambientLight = ambientColor;
}

/*
* @return bool Checks whether the user selected AntiAliasing as a RenderOption
*/
bool World::OPT_ANTI_ALIASING() const
{
	return std::find(renderOptions.begin(), renderOptions.end(), RenderOption::ANTI_ALIASING) != renderOptions.end();
}

/*
* @return bool Checks whether the user selected BVH as a RenderOption
*/
bool World::OPT_BVH() const
{
	return std::find(renderOptions.begin(), renderOptions.end(), RenderOption::BVH) != renderOptions.end();
}

/*
* @return bool Checks whether the user selected TRIANGLE_MESH as a RenderOption
*/
bool World::OPT_TRIANGLE_MESH() const
{
	return std::find(renderOptions.begin(), renderOptions.end(), RenderOption::TRIANGLE_MESH) != renderOptions.end();
}

/*
* @return The background Image of the World
*/
const Image& World::getBackgroundImage()
{
	return backgroundImage;
}

/*
* @return the Camera of the World
*/
const Camera& World::getCamera() const
{
	return camera;
}

/*
* @return the Camera of the World
*/
Camera& World::getCamera()
{
	return camera;
}

/*
* @return The ambient light of the World
*/
const ColorRGB& World::getAmbientLight()
{
	return ambientLight;
}

/*
* Generates a bounding box which surrounds two given bounding boxes
*
* @param box0 The first bounding box
* @param box1 The second bounding box
*
* @return A bounding box surrounding box0 and box1
*/
AABB3D World::surroundingBox(const AABB3D& box0, const AABB3D& box1) const
{
	Point3D small(fmin(box0.min().x(), box1.min().x()),
		fmin(box0.min().y(), box1.min().y()),
		fmin(box0.min().z(), box1.min().z()));

	Point3D big(fmax(box0.max().x(), box1.max().x()),
		fmax(box0.max().y(), box1.max().y()),
		fmax(box0.max().z(), box1.max().z()));

	return AABB3D(small, big);
}

/*
* Generates a bounding box which surrounds ALL given objects
*
* @param objects The list of Objects.
* @param bb The bounding box surrounding all Objects. Modified by function.
*
* @return A bounding box surrounding all Objects in objects.
*/
bool World::surroundingBox(const std::vector<std::shared_ptr<Object>>& objects, AABB3D& bb) const
{
	// No bounding box if list is empty, return false
	if (objects.empty()) return false;

	// Iterate over all objects in the list, find the major bounding box
	// Planes don't have a bounding box, so they will be skipped
	AABB3D temp_box;
	bool first_box = true;
	size_t count = 0;

	for (const auto& objectPtr : objects) {
		Object* currObject = objectPtr.get();
		if (currObject->generateBoundingBox(temp_box)) continue;
		bb = first_box ? temp_box : surroundingBox(bb, temp_box);
		first_box = false;
		count++;
	}

	// If no objects with bounding boxes were encountered, return false
	if (count == 0) {
		return false;
	}

	return true;
}

/*
* Shoots a ray from the origin through the view plane, then determines which objects it hits.
* In a more advanced implementation, this ray will "reflect off" and "pass through" surfaces, meaning there will be multiple intersection points.
* 
* @param currentRow The row of the pixel.
* @param currentColumn The column the pixel.
* @param xOffset The horizontal offset of the ray destination.
* @param yOffset The vertical offset of the ray destination.
* @param firstRay The initial ray shot by the ray tracer. Modified by function.
* @param hitRecord HitRecord struct holding intersection information (if any). Modified by function.
*
* @return Whether or not the ray hit any objects
*/
bool World::shootPrimaryRay(const int& currentRow, const int& currentColumn, const double& xOffset, const double& yOffset, Ray3D& firstRay, HitRecord& hitRecord)
{
	// Direction vector from world camera position to current point on view window
	Point3D firstRayStart;
	Vec3D firstRayDirection;
	camera.getRay(currentRow, currentColumn, xOffset, yOffset, firstRayStart, firstRayDirection);

	// First Ray: From Camera out into the world ...
	firstRay = Ray3D{ firstRayStart, firstRayDirection };

	// finds the index of the SceneObject in the iterable closest to the start of the ray
	double minT = 0;
	double minDist = 0;
	bool intersected = false;

	if (OPT_TRIANGLE_MESH() || OPT_BVH()) {
		return root.intersection(firstRay, 0, MAX_T, hitRecord);
	}

	// Otherwise, just do the usual ...
	for (int i = 0; i < sceneObjects.size(); i++) {
		HitRecord currHitRecord;
		const std::shared_ptr<SceneObject>& currSceneObject = sceneObjects[i];
		bool currIntersected = currSceneObject->intersection(firstRay, 0, MAX_T, currHitRecord);
		if (currIntersected) {
			if (!intersected || currHitRecord.intT < minT) {
				hitRecord = currHitRecord;
				minT = currHitRecord.intT;
			}
			intersected = true;
		}
	}

	return intersected;
}

/*
* Determines the color of the selected pixel. Occurs AFTER primary ray-tracing has been performed.
*
* @param currentRow The row of the pixel.
* @param currentColumn The column the pixel.
* @param intersected The number of intersection points encountered by the primary ray.
* @param firstRay The primary ray.
* @param hitRecord HitRecord struct holding intersection information (if any).
* @param pixelColor A Point3D which will hold the color of the current pixel. Modified by function.
*/
void World::determineColor(const int& currentRow, const int& currentColumn, bool intersected, const Ray3D& firstRay, HitRecord& hitRecord, ColorRGB& pixelColor)
{
	// Just return the background image color if no intersections detected
	if (!intersected) {
		pixelColor = backgroundImage.get(currentRow, currentColumn);
		return;
	}

	// Otherwise, determine color at pixel
	const Point3D& lightRayStart = hitRecord.intPoint;
	bool shadow = false;

	// Color Part 1: Ambient Term
	ColorRGB ambientComponent = (hitRecord.material.ambient / 255).elementMultiply(ambientLight / 255);

	// Color Part 2, 3: Diffuse and Specular Terms
	// Iterate over all objects to check for shadow

	ColorRGB diffuseComponent = BLACK_COLOR / 255;
	ColorRGB specularComponent = BLACK_COLOR / 255;
	for (int i = 0; i < lightSources.size(); i++) {
		const std::shared_ptr<LightSource>& currLightSource = lightSources[i];
		const Point3D currentLightPoint = currLightSource->getLightPoint();
		const Ray3D lightRay{ lightRayStart, currentLightPoint - lightRayStart };

		// Iterate over all objects for the current light source to find shadow
		if (OPT_BVH() || OPT_TRIANGLE_MESH()) {
			const double t_max_shadow = lightRay.getT(currentLightPoint);
			HitRecord shadowHitRecord;
			if (root.intersection(lightRay, 0, t_max_shadow, shadowHitRecord)) {
				shadow = true;
				break;
			}
		}
		else {
			for (int j = 0; j < sceneObjects.size(); j++) {
				const SceneObject* currSceneObject = sceneObjects[j].get();
				HitRecord currLightHitRecord;
				bool currShadow = currSceneObject->intersection(lightRay, 0, MAX_T, currLightHitRecord);

				// intersection happens ONLY IF the intersection point happens BEFORE the ray reaches the light source
				if (currShadow && lightRayStart.distanceTo(currLightHitRecord.intPoint) < lightRayStart.distanceTo(currentLightPoint)) {
					shadow = true;
					diffuseComponent = BLACK_COLOR / 255;
					specularComponent = BLACK_COLOR / 255;
					break;
				}
				// otherwise, apply phong reflection model
			}
			if (shadow) {
				break;
			}
		}

		// If no shadow, apply phong reflection model

		// Color Part 2: Diffuse
		Vec3D N = hitRecord.normal;
		Vec3D L = currentLightPoint - hitRecord.intPoint;
		N.normalize();
		L.normalize();
		Vec3D kd = hitRecord.material.diffuse / 255;
		Vec3D id = currLightSource->getDiffuse() / 255;
		ColorRGB currentDiffuseTerm = (kd.elementMultiply(id)) * std::max((L.dotProduct(N)), 0.0);
		diffuseComponent += currentDiffuseTerm;

		// Color Part 3: Specular (Blinn-Phong)
		Vec3D D = hitRecord.intPoint - currentLightPoint;
		D.normalize();
		Vec3D V = firstRay.getStart() - hitRecord.intPoint;
		V.normalize();
		Vec3D H = L + V;
		H.normalize();
		Vec3D ks = hitRecord.material.specular / 255;
		Vec3D is = currLightSource->getSpecular() / 255;
		const double& alpha = hitRecord.material.alpha;
		ColorRGB currentSpecularTerm = (ks.elementMultiply(is)) * (pow(std::max((N.dotProduct(H)), 0.0), alpha));
		specularComponent += currentSpecularTerm;
	}

	pixelColor = ambientComponent + diffuseComponent + specularComponent;
	pixelColor.capValuesMax({ 1, 1, 1 });
	pixelColor *= RGB_MAX;
}

/*
* Perform ray tracing
* Top-right corner of image is (0,0)
*
* @param currentRow The row of the pixel
* @param currentColumn The column the pixel
* @param xOffset The horizontal offset of the ray destination
* @param yOffset The vertical offset of the ray destination
*
* @return The color derived by the current ray
*
*/
ColorRGB World::rayTrace(const int& currentRow, const int& currentColumn, const double& xOffset, const double& yOffset)
{

	// Step 0: Shoot the primary ray and determine any intersection points
	Ray3D firstRay;
	HitRecord hitRecord;
	Point3D intPoint;
	bool intersected = shootPrimaryRay(currentRow, currentColumn, xOffset, yOffset, firstRay, hitRecord);

	// Step 1: Determine the color of the pixel
	ColorRGB pixelColor;
	determineColor(currentRow, currentColumn, intersected, firstRay, hitRecord, pixelColor);

	return pixelColor;
}

/*
* Render the World into an Image object
*
* @return The rendered Image of the world
*/
Image World::render()
{
	// Preliminary setup
	std::cout << "Total Scene Objects: " << sceneObjects.size() << std::endl;
	std::cout << "Total Light Sources: " << lightSources.size() << std::endl;

	int rows = camera.getViewWindowRows();
	int cols = camera.getViewWindowCols();

	Image rm{ rows, cols };

	int progressBlock = (rows * cols) / 100;
	size_t blockCount = 0;

	camera.ready();

	// Record Start time
	auto start_time = std::chrono::high_resolution_clock::now();

	// Preliminary: Prepare BVH Tree
	clock_t start;
	start = clock();

	if (OPT_TRIANGLE_MESH()) {
		std::cout << std::endl << "Building BVH from TriangleMesh..." << std::endl;
		root = BVHNode(triangleMesh);
	} else if (OPT_BVH()) {
		std::cout << std::endl << "Building BVH from sceneObjects..." << std::endl;
		root = BVHNode(sceneObjects);
	}

	// Record BVH time (if applicable)
	double bvh_seconds = 0;
	auto bvh_finish_time = clock();
	if (OPT_TRIANGLE_MESH() || OPT_BVH()) {
		bvh_seconds = ((double)(bvh_finish_time - start)) / ((double)(CLOCKS_PER_SEC));
		std::cout << "Done building BVH! Took " << bvh_seconds << " seconds." << std::endl << std::endl;
	}

	std::cout << std::endl;

	// Iterate over all pixels, perform ray tracing on each
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {

			// Generate multiple samples with anti-aliasing, one sample without
			std::vector<std::pair<double, double>> offsets{};
			if (OPT_ANTI_ALIASING()) {
				Arithmetic::multi_jittered_sampling(4, offsets);
			}
			else {
				offsets.push_back(std::make_pair(0.5, 0.5));
			}
			std::vector<ColorRGB> colors;
			for (std::pair<double, double> o : offsets) {
				colors.push_back(rayTrace(i, j, o.first, o.second));
			}

			// Take average of all colors for current pixel
			rm.set(i, j, Arithmetic::averageVec3D(colors));

			if ((i * rows + j) % progressBlock == 0) {
				std::cout << "Rendering ... " << blockCount << "% done" << std::endl;
				blockCount++;
			}
		}
	}
	std::cout << "Rendering ... " << blockCount << "% done" << std::endl;

	// Record Ending times
	double ray_tracing_seconds = 0;
	auto render_finish_time = clock();
	ray_tracing_seconds = ((double)(render_finish_time - bvh_finish_time)) / ((double)(CLOCKS_PER_SEC));

	double total_render_seconds = ray_tracing_seconds;
	
	std::cout << "Statistics:" << std::endl;
	if (OPT_TRIANGLE_MESH() || OPT_BVH()) {
		std::cout << "BVH Construction Time: " << bvh_seconds << " seconds." << std::endl;
		total_render_seconds += bvh_seconds;
	}
	std::cout << "Ray Tracing Time: " << ray_tracing_seconds << " seconds." << std::endl;
	std::cout << "Total Render Time: " << total_render_seconds << " seconds." << std::endl << std::endl;

	// Return rendered image
	return rm;
}