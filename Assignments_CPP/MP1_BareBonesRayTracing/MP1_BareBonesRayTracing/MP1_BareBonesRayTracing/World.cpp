#include "World.h"

/*
* Default constructor for World
*/
World::World()  {}

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
	// Direction vector from world camera position to current point on view window
	Point3D firstRayStart;
	Vec3D firstRayDirection;
	camera.getRay(currentRow, currentColumn, xOffset, yOffset, firstRayStart, firstRayDirection);

	// First Ray: From Camera out into the world ...
	const Ray3D firstRay{ firstRayStart, firstRayDirection };

	// finds the index of the SceneObject in the iterable closest to the start of the ray
	std::vector<Point3D> intPoints;
	int closestObjectIdx = 0;
	double closestDistance = 0;
	bool intersected = false;
	Point3D closestIntPoint;
	for (int i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i].get()->intersection(firstRay, intPoints);
		if (intPoints.size() > 0) {
			int closestIntPointIdx = Arithmetic::closestPoint(firstRayStart, intPoints);
			double dist = firstRayStart.distanceTo(intPoints[closestIntPointIdx]);
			if (!intersected || dist < closestDistance) {
				closestObjectIdx = i;
				closestDistance = dist;
				closestIntPoint = intPoints[closestIntPointIdx];
			}
			intersected = true;
		}
		intPoints.clear();
	}

	// just return the background image color if no intersections detected
	if (!intersected) {
		return backgroundImage.get(currentRow, currentColumn);
	}

	// otherwise, determine color at pixel
	const SceneObject* closestObject = sceneObjects[closestObjectIdx].get();
	const Point3D& lightRayStart = closestIntPoint;
	bool shadow = false;

	// Color Part 1: Ambient Term
	ColorRGB ambientComponent = (closestObject->getAmbient() / 255).elementMultiply(ambientLight / 255);

	// Color Part 2, 3: Diffuse and Specular Terms
	// Iterate over all objects to check for shadow
	ColorRGB diffuseComponent = BLACK_COLOR / 255;
	ColorRGB specularComponent = BLACK_COLOR / 255;
	for (int i = 0; i < lightSources.size(); i++) {
		const LightSource* currLightSource = lightSources[i].get();
		const Point3D currentLightPoint = currLightSource->getLightPoint();
		const Ray3D lightRay{ lightRayStart, currentLightPoint - lightRayStart };

		// iterate over all objects for the current light source to find shadow
		for (int j = 0; j < sceneObjects.size(); j++) {
			const SceneObject* currSceneObject = sceneObjects[j].get();
			currSceneObject->intersection(lightRay, intPoints);
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

		// if no shadow, apply phong reflection model

		// Color Part 2: Diffuse
		Vec3D N = closestObject->normal(closestIntPoint);
		Vec3D L = currentLightPoint - closestIntPoint;
		N.normalize();
		L.normalize();
		Vec3D kd = closestObject->getDiffuse() / 255;
		Vec3D id = currLightSource->getDiffuse() / 255;
		ColorRGB currentDiffuseTerm = (kd.elementMultiply(id)) * std::max((L.dotProduct(N)), 0.0);
		diffuseComponent += currentDiffuseTerm;

		// Color Part 3: Specular
		Vec3D D = closestIntPoint - currentLightPoint;
		D.normalize();
		Vec3D V = firstRayStart - closestIntPoint;
		V.normalize();
		//Vec3D R = D - (N * (D.dotProduct(N)) * 2);
		//R.normalize();
		Vec3D H = L + V;
		H.normalize();
		Vec3D ks = closestObject->getSpecular() / 255;
		Vec3D is = currLightSource->getSpecular() / 255;
		const double& alpha = closestObject->getAlpha();
		ColorRGB currentSpecularTerm = (ks.elementMultiply(is)) * (pow(std::max((N.dotProduct(H)), 0.0), alpha));
		specularComponent += currentSpecularTerm;
	}

	ColorRGB pixelColor = ambientComponent + diffuseComponent + specularComponent;
	pixelColor.capValuesMax({ 1, 1, 1 });
	pixelColor *= RGB_MAX;
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

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
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
			rm.set(i, j, Arithmetic::averageVec3D(colors));
			if (i % progressX == 0 && j % progressY == 0) {
				std::cout << "Rendering ... " << 10 * (i / progressX) + (j / progressY) << "% done" << std::endl;
			}
		}
	}

	return rm;
}

// ARCHIVE

//ColorRGB World::rayTrace(const int& currentRow, const int& currentColumn, const double& xOffset, const double& yOffset)
//{
//	// Direction vector from world camera position to current point on view window
//	Point3D firstRayStart;
//	Vec3D firstRayDirection;
//	camera.getRay(currentRow, currentColumn, xOffset, yOffset, firstRayStart, firstRayDirection);
//
//	//std::cout << dir.toString() << std::endl;
//
//	// First Ray: From Camera out into the world ...
//	const Ray3D firstRay{ firstRayStart, firstRayDirection };
//
//	// finds the index of the SceneObject in the iterable closest to the start of the ray
//	std::vector<Point3D> intPoints;
//	int closestObjectIdx = 0;
//	double closestDistance = 0;
//	bool intersected = false;
//	Point3D closestIntPoint;
//	for (int i = 0; i < sceneObjects.size(); i++) {
//		sceneObjects[i].get()->intersection(firstRay, intPoints);
//		if (intPoints.size() > 0) {
//			int closestIntPointIdx = Arithmetic::closestPoint(firstRayStart, intPoints);
//			double dist = firstRayStart.distanceTo(intPoints[closestIntPointIdx]);
//			if (!intersected || dist < closestDistance) {
//				closestObjectIdx = i;
//				closestDistance = dist;
//				closestIntPoint = intPoints[closestIntPointIdx];
//			}
//			intersected = true;
//		}
//		intPoints.clear();
//	}
//
//	// just return the background image color if no intersections detected
//	if (!intersected) {
//		return backgroundImage.get(currentRow, currentColumn);
//	}
//
//	// otherwise, perform shadow traving
//	const SceneObject* closestObject = sceneObjects[closestObjectIdx].get();
//	const Point3D& lightRayStart = closestIntPoint;
//	double shadowFactor = 1.0;
//	bool shadow = false;
//
//	// iterate over all light sources
//	for (int i = 0; i < lightSources.size(); i++) {
//		const LightSource* currLightSource = lightSources[i].get();
//		const Ray3D lightRay{ lightRayStart, currLightSource->getLightPoint() - lightRayStart };
//		const Point3D currentLightPoint = currLightSource->getLightPoint();
//
//		// iterate over all objects for the current light soirce
//		for (int j = 0; j < sceneObjects.size(); j++) {
//			const SceneObject* currSceneObject = sceneObjects[j].get();
//			currSceneObject->intersection(lightRay, intPoints);
//			const int closestShadowIntPointIdx = Arithmetic::closestPoint(lightRayStart, intPoints);
//
//			//if (currSceneObject->getObjectType() == ObjectType::Triangle) {
//			//	std::cout << "tri: " << lightRay.getStart().toString() << std::endl;
//			//}
//
//			// intersection happens ONLY IF the intersection point happens BEFORE the ray reaches the light source
//			if (intPoints.size() > 0 && lightRayStart.distanceTo(intPoints[closestShadowIntPointIdx]) < lightRayStart.distanceTo(currentLightPoint)) {
//
//				shadowFactor *= 0.5;
//				shadow = true;
//				break;
//			}
//			// otherwise, brighten the pixel (unless max brightness is reached)
//			else {
//				shadowFactor = std::min(1.0, shadowFactor * (1.0 + currLightSource->getBrightness()));
//			}
//			intPoints.clear();
//		}
//		intPoints.clear();
//	}
//
//	ColorRGB returnColor = closestObject->getColor();
//	returnColor.changeBrightness(shadowFactor);
//	return returnColor;
//}
