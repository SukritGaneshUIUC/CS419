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
bool World::antiAliasing() const
{
	return std::find(renderOptions.begin(), renderOptions.end(), RenderOption::ANTI_ALIASING) != renderOptions.end();
}

/*
* @return bool Checks whether the user selected BVH as a RenderOption
*/
bool World::BVH() const
{
	return std::find(renderOptions.begin(), renderOptions.end(), RenderOption::BVH) != renderOptions.end();
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

bool World::bvhIntersectionHelper(const BVHNode& currNode, const Ray3D& firstRay, std::vector<std::shared_ptr<SceneObject>>& intersectedObjects, std::vector<Point3D>& intersectionPoints) {
	std::vector<Point3D> intPoints;
	//std::cout << "goth" << std::endl;

	// Break if current node isn't hit
	if (!currNode.intersection(firstRay, 0, MAX_T, intPoints)) {
		//AABB3D bb;
		//currNode.generateBoundingBox(bb);
		//std::cout << bb.toString() << std::endl;
		//std::cout << "no int in node" << std::endl;
		return false;
	}
	//std::cout << "int" << std::endl;
	intPoints.clear();

	// If current node is leaf, perform ray tracing intersection on node's objects
	if (currNode.isLeaf()) {
		std::shared_ptr<SceneObject> left = std::dynamic_pointer_cast<SceneObject>(currNode.getLeft());
		std::shared_ptr<SceneObject> right = std::dynamic_pointer_cast<SceneObject>(currNode.getLeft());
		bool intLeft = false;
		bool intRight = false;

		// check left leaf intersection
		intLeft = left->intersection(firstRay, 0, MAX_T, intPoints);
		if (intLeft) {
			size_t leftIntIdx = Arithmetic::closestPoint(firstRay.getStart(), intPoints);
			intersectedObjects.push_back(left);
			intersectionPoints.push_back(intPoints[leftIntIdx]);
			intPoints.clear();
		}

		// Return early if left and right point to same SceneObject
		if (left.get() == right.get()) {
			return intLeft;
		}

		// check right leaf intersection
		intRight = right->intersection(firstRay, 0, MAX_T, intPoints);
		if (intRight) {
			size_t rightIntIdx = Arithmetic::closestPoint(firstRay.getStart(), intPoints);
			intersectedObjects.push_back(right);
			intersectionPoints.push_back(intPoints[rightIntIdx]);
		}
		
		return intLeft || intRight;
	}
	//std::cout << "notleaf" << std::endl;

	// Otherwise, check left and right nodes
	const BVHNode left = *(std::dynamic_pointer_cast<BVHNode>(currNode.getLeft()));
	const BVHNode right = *(std::dynamic_pointer_cast<BVHNode>(currNode.getRight()));

	bool intLeft = false;
	bool intRight = false;

	intLeft = bvhIntersectionHelper(left, firstRay, intersectedObjects, intersectionPoints);
	intRight = bvhIntersectionHelper(right, firstRay, intersectedObjects, intersectionPoints);

	return intLeft || intRight;
}

bool World::bvhIntersection(const Ray3D& firstRay, std::shared_ptr<SceneObject>& intersectedObject, Point3D& intersectionPoint)
{
	// Get all intersections by traversing BVH Tree
	std::vector<std::shared_ptr<SceneObject>> intersectedObjects;
	std::vector<Point3D> intersectionPoints;
	//std::cout << "starting inthelper" << std::endl;
	bool intersected = bvhIntersectionHelper(root, firstRay, intersectedObjects, intersectionPoints);
	//std::cout << "ending inthelper" << std::endl;
	if (intersectedObjects.size() == 0) {
		//std::cout << firstRay.getDirection().toString() << " noint" << std::endl;
		return false;
	}

	//std::cout << "here" << std::endl;

	// Find closest object
	bool first = true;
	double closestDist = 0;

	for (int i = 0; i < intersectedObjects.size(); i++) {
		const Point3D& currIntPoint = intersectionPoints[i];
		double currDist = firstRay.getStart().distanceTo(currIntPoint);
		if (first || currDist < closestDist) {
			closestDist = currDist;
			intersectedObject = intersectedObjects[i];
			intersectionPoint = currIntPoint;
			first = false;
		}
	}

	//std::cout << "done" << std::endl;

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
* @param firstRayStart A Point3D which will hold the origin of the primary ray. Modified by function.
* @param intersectedObject The closest intersected object (if any). Modified by function.
* @param intersectionPoint A Point3D which will hold the intersection point closest to the ray origin (if any). Modified by function.
*
* @return Whether or not the ray hit any objects
*/
bool World::shootPrimaryRay(const int& currentRow, const int& currentColumn, const double& xOffset, const double& yOffset, Point3D& firstRayStart, std::shared_ptr<SceneObject>& intersectedObject, Point3D& intersectionPoint)
{
	// Direction vector from world camera position to current point on view window
	Vec3D firstRayDirection;
	camera.getRay(currentRow, currentColumn, xOffset, yOffset, firstRayStart, firstRayDirection);

	// First Ray: From Camera out into the world ...
	const Ray3D firstRay{ firstRayStart, firstRayDirection };

	// finds the index of the SceneObject in the iterable closest to the start of the ray
	std::vector<Point3D> intPoints;
	double closestDistance = 0;
	bool intersected = false;

	// If BVH, use it
	if (BVH()) {
		return bvhIntersection(firstRay, intersectedObject, intersectionPoint);
	}

	// Otherwise, just do the usual ...
	for (int i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i].get()->intersection(firstRay, 0, MAX_T, intPoints);
		if (intPoints.size() > 0) {
			int closestIntPointIdx = Arithmetic::closestPoint(firstRayStart, intPoints);
			double dist = firstRayStart.distanceTo(intPoints[closestIntPointIdx]);
			if (!intersected || dist < closestDistance) {
				intersectedObject = sceneObjects[i];
				closestDistance = dist;
				intersectionPoint = intPoints[closestIntPointIdx];
			}
			intersected = true;
		}
		intPoints.clear();
	}

	return intersected;
}

/*
* Determines the color of the selected pixel. Occurs AFTER primary ray-tracing has been performed.
*
* @param currentRow The row of the pixel.
* @param currentColumn The column the pixel.
* @param intersected The number of intersection points encountered by the primary ray.
* @param firstRayStart A Point3D which will hold the origin of the primary ray. Modified by function.
* @param closestObjectIdx The index of the closest intersected object (if any).
* @param intersectionPoint The intersection point closest to the ray origin (if any).
* @param closestObjectIdx An integer which will hold the index of the closest intersected object (if any). 
* @param pixelColor A Point3D which will hold the color of the current pixel. Modified by function.
*
* @return Whether or not the ray hit any objects
*/
void World::determineColor(const int& currentRow, const int& currentColumn, bool intersected, const Point3D& firstRayStart, const std::shared_ptr<SceneObject>& intersectedObject, const Point3D& intersectionPoint, ColorRGB& pixelColor)
{
	// Just return the background image color if no intersections detected
	if (!intersected) {
		pixelColor = backgroundImage.get(currentRow, currentColumn);
		return;
	}

	// Otherwise, determine color at pixel
	std::vector<Point3D> intPoints;
	const Point3D& lightRayStart = intersectionPoint;
	bool shadow = false;

	// Color Part 1: Ambient Term
	ColorRGB ambientComponent = (intersectedObject->getAmbient() / 255).elementMultiply(ambientLight / 255);

	// Color Part 2, 3: Diffuse and Specular Terms
	// Iterate over all objects to check for shadow

	ColorRGB diffuseComponent = BLACK_COLOR / 255;
	ColorRGB specularComponent = BLACK_COLOR / 255;
	for (int i = 0; i < lightSources.size(); i++) {
		const LightSource* currLightSource = lightSources[i].get();
		const Point3D currentLightPoint = currLightSource->getLightPoint();
		const Ray3D lightRay{ lightRayStart, currentLightPoint - lightRayStart };

		// Iterate over all objects for the current light source to find shadow
		for (int j = 0; j < sceneObjects.size(); j++) {
			const SceneObject* currSceneObject = sceneObjects[j].get();
			currSceneObject->intersection(lightRay, 0, MAX_T, intPoints);
			const int closestShadowIntPointIdx = Arithmetic::closestPoint(lightRayStart, intPoints);

			// intersection happens ONLY IF the intersection point happens BEFORE the ray reaches the light source
			if (intPoints.size() > 0 && lightRayStart.distanceTo(intPoints[closestShadowIntPointIdx]) < lightRayStart.distanceTo(currentLightPoint)) {
				shadow = true;
				diffuseComponent = BLACK_COLOR / 255;
				specularComponent = BLACK_COLOR / 255;
				break;
			}
			// otherwise, apply phong reflection model
			intPoints.clear();
		}
		intPoints.clear();
		if (shadow) {
			break;
		}

		// If no shadow, apply phong reflection model

		// Color Part 2: Diffuse
		Vec3D N = intersectedObject->normal(intersectionPoint);
		Vec3D L = currentLightPoint - intersectionPoint;
		N.normalize();
		L.normalize();
		Vec3D kd = intersectedObject->getDiffuse() / 255;
		Vec3D id = currLightSource->getDiffuse() / 255;
		ColorRGB currentDiffuseTerm = (kd.elementMultiply(id)) * std::max((L.dotProduct(N)), 0.0);
		diffuseComponent += currentDiffuseTerm;

		// Color Part 3: Specular (Blinn-Phong)
		Vec3D D = intersectionPoint - currentLightPoint;
		D.normalize();
		Vec3D V = firstRayStart - intersectionPoint;
		V.normalize();
		Vec3D H = L + V;
		H.normalize();
		Vec3D ks = intersectedObject->getSpecular() / 255;
		Vec3D is = currLightSource->getSpecular() / 255;
		const double& alpha = intersectedObject->getAlpha();
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
	Point3D firstRayStart;
	Vec3D firstRayDirection;
	std::shared_ptr<SceneObject> intersectedObject;
	Point3D intPoint;
	bool intersected = shootPrimaryRay(currentRow, currentColumn, xOffset, yOffset, firstRayStart, intersectedObject, intPoint);
	camera.getRay(currentRow, currentColumn, xOffset, yOffset, firstRayStart, firstRayDirection);

	// Step 1: Determine the color of the pixel
	ColorRGB pixelColor;
	determineColor(currentRow, currentColumn, intersected, firstRayStart, intersectedObject, intPoint, pixelColor);

	return pixelColor;
}

/*
* Render the World into an Image object
*
* @return The rendered Image of the world
*/
Image World::render()
{
	std::cout << "Total Scene Objects: " << sceneObjects.size() << std::endl;
	std::cout << "Total Light Sources: " << lightSources.size() << std::endl;

	int rows = camera.getViewWindowRows();
	int cols = camera.getViewWindowCols();

	Image rm{ rows, cols };

	int progressX = cols / 10;
	int progressY = rows / 10;

	camera.ready();

	// Preliminary: Prepare BVH Tree
	if (BVH()) {
		root = BVHNode(sceneObjects);
	}

	std::cout << std::endl;
	// Iterate over all pixels, perform ray tracing on each
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {

			// Generate multiple samples with anti-aliasing, one sample without
			std::vector<std::pair<double, double>> offsets{};
			if (antiAliasing()) {
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

			if (i % progressX == 0 && j % progressY == 0) {
				std::cout << "Rendering ... " << 10 * (i / progressX) + (j / progressY) << "% done" << std::endl;
			}
		}
	}

	return rm;
}









//bool hitLeft = left.intersection(firstRay, 0, 10000, intPoints);
//bool hitRight = right.intersection(firstRay, 0, 10000, intPoints);

//bool intLeft = false;
//bool intRight = false;

//if (hitLeft) {
//	intLeft = bvhIntersectionHelper(left, firstRay, intersectedObjects, intersectionPoints);
//	intPoints.clear();
//}

//if (hitRight) {
//	intRight = bvhIntersectionHelper(right, firstRay, intersectedObjects, intersectionPoints);
//}















///*
//* Shoots a ray from the origin through the view plane, then determines which objects it hits.
//* In a more advanced implementation, this ray will "reflect off" and "pass through" surfaces, meaning there will be multiple intersection points.
//*
//* @param currentRow The row of the pixel.
//* @param currentColumn The column the pixel.
//* @param xOffset The horizontal offset of the ray destination.
//* @param yOffset The vertical offset of the ray destination.
//* @param firstRay The primary ray shot by the ray tracer. Modified by function.
//* @param intersectedObjectIdx An integer which will hold the index of the closest intersected object (if any). Modified by function.
//* @param intersectionT A double which will hold the ray's t-value closest to the ray origin (if any). Modified by function.
//*
//* @return Whether or not the ray hit any objects
//*/
//bool World::shootPrimaryRay(const size_t& currentRow, const size_t& currentColumn, const double& xOffset, const double& yOffset, Ray3D& firstRay, size_t& intersectedObjectIdx, double& intersectionT)
//{
//	// Direction vector from world camera position to current point on view window
//	Vec3D firstRayStart;
//	Vec3D firstRayDirection;
//	camera.getRay(currentRow, currentColumn, xOffset, yOffset, firstRayStart, firstRayDirection);
//
//	// First Ray: From Camera out into the world ...
//	firstRay = Ray3D{ firstRayStart, firstRayDirection };
//
//	// finds the index of the SceneObject in the iterable closest to the start of the ray
//	double closestDistance = 0;
//	bool intersected = false;
//	for (int i = 0; i < sceneObjects.size(); i++) {
//		std::vector<double> intTs;
//		sceneObjects[i].get()->intersection(firstRay, 0, 0, intTs);
//		if (intTs.size() > 0) {
//			size_t currIntersectionT = *(std::min_element(intTs.begin(), intTs.end()));
//			if (!intersected || currIntersectionT < intersectionT) {
//				intersectedObjectIdx = i;
//				intersectionT = currIntersectionT;
//			}
//			intersected = true;
//		}
//	}
//
//	return intersected;
//}
//
///*
//* Determines the color of the selected pixel. Occurs AFTER primary ray-tracing has been performed.
//*
//* @param currentRow The row of the pixel.
//* @param currentColumn The column the pixel.
//* @param intersected The number of intersection points encountered by the primary ray.
//* @param firstRay A Ray3D which will hold primary ray.
//* @param intersectedObjectIdx The index of the primary ray's intersected object (if any).
//* @param intersectionT The "t" value when the primary ray intersects an object (if any).
//* @param closestObjectIdx An integer which will hold the index of the closest intersected object (if any).
//* @param pixelColor A Point3D which will hold the color of the current pixel. Modified by function.
//*
//* @return Whether or not the ray hit any objects
//*/
//void World::determineColor(const size_t& currentRow, const size_t& currentColumn, bool intersected, const Ray3D& firstRay, const size_t& intersectedObjectIdx, const double& intersectionT, ColorRGB& pixelColor)
//{
//	// Just return the background image color if no intersections detected
//	if (!intersected) {
//		pixelColor = backgroundImage.get(currentRow, currentColumn);
//		return;
//	}
//
//	// Otherwise, determine color at pixel
//	const Point3D intersectionPoint = firstRay.pos(intersectionT);
//	const SceneObject* intersectedObject = sceneObjects[intersectedObjectIdx].get();
//	const Point3D lightRayStart = firstRay.pos(intersectionT);
//	bool shadow = false;
//
//	// Color Part 1: Ambient Term
//	ColorRGB ambientComponent = (intersectedObject->getAmbient() / 255).elementMultiply(ambientLight / 255);
//
//	// Color Part 2, 3: Diffuse and Specular Terms
//	// Iterate over all objects to check for shadow
//
//	ColorRGB diffuseComponent = BLACK_COLOR / 255;
//	ColorRGB specularComponent = BLACK_COLOR / 255;
//	for (int i = 0; i < lightSources.size(); i++) {
//
//		const LightSource* currentLightSource = lightSources[i].get();
//		const Point3D currentLightPoint = currentLightSource->getLightPoint();
//		const Ray3D lightRay{ lightRayStart, currentLightPoint - lightRayStart };
//
//		// Iterate over all objects for the current light source to find shadow
//		for (int j = 0; j < sceneObjects.size(); j++) {
//			const SceneObject* currSceneObject = sceneObjects[j].get();
//			std::vector<double> intTs;
//			currSceneObject->intersection(lightRay, 0, 0, intTs);
//
//			// intersection happens ONLY IF the intersection point happens BEFORE the ray reaches the light source
//			if (intTs.size() > 0) {
//				size_t closestShadowIntT = *(std::min_element(intTs.begin(), intTs.end()));
//				if (closestShadowIntT < lightRay.getT(currentLightPoint)) {
//					shadow = true;
//					diffuseComponent = BLACK_COLOR / 255;
//					specularComponent = BLACK_COLOR / 255;
//					break;
//				}
//			}
//			if (shadow) {
//				break;
//			}
//		}
//		if (shadow) {
//			break;
//		}
//
//		// If no shadow, apply phong reflection model
//
//		// Color Part 2: Diffuse
//		Vec3D N = intersectedObject->normal(intersectionPoint);
//		Vec3D L = currentLightPoint - intersectionPoint;
//		N.normalize();
//		L.normalize();
//		Vec3D kd = intersectedObject->getDiffuse() / 255;
//		Vec3D id = currentLightSource->getDiffuse() / 255;
//		ColorRGB currentDiffuseTerm = (kd.elementMultiply(id)) * std::max((L.dotProduct(N)), 0.0);
//		diffuseComponent += currentDiffuseTerm;
//
//		// Color Part 3: Specular (Blinn-Phong)
//		Vec3D D = intersectionPoint - currentLightPoint;
//		D.normalize();
//		Vec3D V = firstRay.getStart() - intersectionPoint;
//		V.normalize();
//		Vec3D H = L + V;
//		H.normalize();
//		Vec3D ks = intersectedObject->getSpecular() / 255;
//		Vec3D is = currentLightSource->getSpecular() / 255;
//		const double& alpha = intersectedObject->getAlpha();
//		ColorRGB currentSpecularTerm = (ks.elementMultiply(is)) * (pow(std::max((N.dotProduct(H)), 0.0), alpha));
//		specularComponent += currentSpecularTerm;
//	}
//
//	pixelColor = ambientComponent + diffuseComponent + specularComponent;
//	pixelColor.capValuesMax({ 1, 1, 1 });
//	pixelColor *= RGB_MAX;
//}
//
///*
//* Perform ray tracing
//* Top-right corner of image is (0,0)
//*
//* @param currentRow The row of the pixel
//* @param currentColumn The column the pixel
//* @param xOffset The horizontal offset of the ray destination
//* @param yOffset The vertical offset of the ray destination
//*
//* @return The color derived by the current ray
//*
//*/
//ColorRGB World::rayTrace(const int& currentRow, const int& currentColumn, const double& xOffset, const double& yOffset)
//{
//	// Step 0: Shoot the primary ray and determine any intersection points
//	Ray3D firstRay;
//	size_t intersectedObjectIdx;
//	double intersectionT;
//	bool intersected = shootPrimaryRay(currentRow, currentColumn, xOffset, yOffset, firstRay, intersectedObjectIdx, intersectionT);
//
//	// Step 1: Determine the color of the pixel
//	ColorRGB pixelColor;
//	determineColor(currentRow, currentColumn, intersected, firstRay, intersectedObjectIdx, intersectionT, pixelColor);
//
//	return pixelColor;
//}