#include "World.h"
#include "Mirror.h"
#include "Dielectric.h"

/*
* Default constructor for World
*/
World::World() {}

/*
* Default destructor for World
*/
World::~World() {}

/*
* @return A reference to the vector of all scene Objects in the World
*/
const std::vector<std::shared_ptr<Object>>& World::getSceneObjects() const
{
	return sceneObjects;
}

/*
* @return A reference to the world's light source
*/
const std::shared_ptr<AreaLightSource> World::getLightSource() const
{
	return lightSource;
}

/*
* @return A reference to the vector of all RenderOption in the World
*/
const std::vector<RenderOption>& World::getRenderOptions() const
{
	return renderOptions;
}

/*
* @param sceneObject The scene Object to add to the world
*/
void World::addSceneObject(std::shared_ptr<Object> sceneObject)
{

	sceneObjects.push_back(sceneObject);
}

/*
* @param lightSource The light source Object to add to the world
*/
void World::addLightSource(std::shared_ptr<AreaLightSource> lightSource)
{
	this->lightSource = lightSource;
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
* Determines what objects a ray intersects..
*
* @param firstRay The ray to trace.
* @param hitRecord HitRecord struct holding intersection information (if any). Modified by function.
*
* @return Whether or not the ray hit any objects
*/
bool World::shootRay(const Ray3D& firstRay, HitRecord& hitRecord)
{
	// finds the index of the SceneObject in the iterable closest to the start of the ray
	double minT = 0;
	double minDist = 0;
	bool intersected = false;

	// Just do the usual ...
	for (int i = 0; i < sceneObjects.size(); i++) {
		if (intersected) {
			sceneObjects[i]->intersection(firstRay, 0, hitRecord.intT, hitRecord);
		}
		else {
			intersected = sceneObjects[i]->intersection(firstRay, 0, MAX_T, hitRecord);
		}
	}

	bool lightIntersected = false;
	if (intersected) {
		lightIntersected = lightSource->intersection(firstRay, 0, hitRecord.intT, hitRecord);
	}
	else {
		lightIntersected = lightSource->intersection(firstRay, 0, MAX_T, hitRecord);
	}
	if (lightIntersected) {
		hitRecord.lightSource = true;
	}

	return intersected;
}

/*
* Determines the color of the selected pixel. Occurs AFTER primary ray-tracing has been performed.
*
* @param ray The ray.
* @param hitRecord HitRecord struct holding intersection information.
* @param pixelColor A Point3D which will hold the color of the current pixel. Modified by function.
*/
void World::blinnPhongShading(const Ray3D& ray, const HitRecord& hitRecord, ColorRGB& pixelColor)
{
	// Just return the background image color if no intersections detected
	if (!hitRecord.intersected) {
		pixelColor = backgroundImage.get(0, 0);
		return;
	}

	// Just return the light color if the ray has hit the light
	if (hitRecord.lightSource) {
		pixelColor = lightSource->getMaterial()->diffuse;
		return;
	}

	// Iterate over all sample points on the area light source!!!
	std::vector<Point3D> samplePoints = lightSource->getSamplePoints();
	ColorRGB runningColorSum{ 0,0,0 };

	for (size_t i = 0; i < samplePoints.size(); i++) {

		const Point3D currLightPoint = samplePoints[i];
		const Point3D& lightRayStart = hitRecord.intPoint;
		ColorRGB currColor{ 0,0,0 };
		ColorRGB diffuseComponent = BLACK_COLOR / 255;
		ColorRGB specularComponent = BLACK_COLOR / 255;
		const Ray3D lightRay{ lightRayStart, currLightPoint - lightRayStart };

		// Color Part 1: Ambient Term
		ColorRGB ambientComponent = (hitRecord.material->ambient / 255).elementMultiply(ambientLight / 255);

		// Color Part 2, 3: Diffuse and Specular Terms
		// Iterate over all objects to check for shadow

		// Iterate over all objects for the current light source to find shadow
		bool shadow = false;
		double maxShadowT = lightRay.getT(currLightPoint);
		for (int j = 0; j < sceneObjects.size(); j++) {

			// intersection happens ONLY IF the intersection point happens BEFORE the ray reaches the light source
			HitRecord shadowHitRecord;
			bool currShadow = sceneObjects[j]->intersection(lightRay, 0, maxShadowT, shadowHitRecord);
			if (currShadow) {
				shadow = true;
				break;
			}
		}

		if (shadow) {
			currColor = ambientComponent;
			currColor.capValuesMax({ 1, 1, 1 });
			currColor *= RGB_MAX;
			runningColorSum += currColor;
			continue;
		}

		// If no shadow, apply phong reflection model

		// Color Part 2: Diffuse
		Vec3D N = hitRecord.normal;
		Vec3D L = currLightPoint - hitRecord.intPoint;
		N.normalize();
		L.normalize();
		Vec3D kd = hitRecord.material->diffuse / 255;
		Vec3D id = lightSource->getMaterial()->diffuse / 255;
		ColorRGB currentDiffuseTerm = (kd.elementMultiply(id)) * std::max((L.dotProduct(N)), 0.0);
		diffuseComponent += currentDiffuseTerm;

		// Color Part 3: Specular (Blinn-Phong)
		Vec3D D = hitRecord.intPoint - currLightPoint;
		D.normalize();
		Vec3D V = ray.getStart() - hitRecord.intPoint;
		V.normalize();
		Vec3D H = L + V;
		H.normalize();
		Vec3D ks = hitRecord.material->specular / 255;
		Vec3D is = lightSource->getMaterial()->specular / 255;
		const double& alpha = hitRecord.material->alpha;
		ColorRGB currentSpecularTerm = (ks.elementMultiply(is)) * (pow(std::max((N.dotProduct(H)), 0.0), alpha));
		specularComponent += currentSpecularTerm;

		currColor = ambientComponent + diffuseComponent + specularComponent;
		currColor.capValuesMax({ 1, 1, 1 });
		currColor *= RGB_MAX;
		runningColorSum += currColor;
	}

	runningColorSum /= samplePoints.size();
	pixelColor = runningColorSum;
}

ColorRGB World::rayTracerHelper(const Ray3D& ray, double depth) {
	rays_shot++;

	// Intersect the ray with all objects (including light)
	HitRecord hitRecord;
	shootRay(ray, hitRecord);

	// Run blinn-phong
	ColorRGB blinnPhongComponent;
	blinnPhongShading(ray, hitRecord, blinnPhongComponent);

	// Just return blinn-phong color under these conditions
	if (depth == 0 || hitRecord.intersected == false || hitRecord.material->materialType == MaterialType::SOLID) {
		return blinnPhongComponent;
	}

	// Deal with reflective surfaces
	else if (hitRecord.material->materialType == MaterialType::REFLECTIVE) {
		std::shared_ptr<Mirror> m = std::dynamic_pointer_cast<Mirror>(hitRecord.material);
		Vec3D reflectionVector{ ray.getDirection().reflect(hitRecord.normal) };
		Ray3D reflectionRay{ hitRecord.intPoint, reflectionVector };
		ColorRGB reflectionComponent = rayTracerHelper(reflectionRay, depth - 1);
		//return (reflectionComponent * m->reflectivity) + (blinnPhongComponent * (1 - m->reflectivity));
		return reflectionComponent * DEFAULT_REFLECTIVITY + blinnPhongComponent * (1.0 - DEFAULT_REFLECTIVITY);

	}

	// Deal with dielectric surfaces
	else if (hitRecord.material->materialType == MaterialType::DIELECTRIC) {
		std::shared_ptr<Dielectric> d = std::dynamic_pointer_cast<Dielectric>(hitRecord.material);
		Vec3D refractionVector;
		int refret = 0;
		if (hitRecord.front_face) {
			refret = Arithmetic::refract(ray.getDirection(), hitRecord.normal, DEFAULT_REFRACTIVE_INDEX, refractionVector);
		}
		else {
			refret = Arithmetic::refract(ray.getDirection(), hitRecord.normal * -1, 1.0 / DEFAULT_REFRACTIVE_INDEX, refractionVector);
		}

		// do refraction if total internal reflection doesn't happen, just do blinn phong otherwise
		if (refret) {
			Ray3D refractionRay{ hitRecord.intPoint, refractionVector };
			ColorRGB refractionComponent = rayTracerHelper(refractionRay, depth - 1);
			return refractionComponent * DEFAULT_TRANSMISSION_COEFFICIENT + blinnPhongComponent * (1.0 - DEFAULT_TRANSMISSION_COEFFICIENT);
		}
		return blinnPhongComponent;
		
	}

	// Everything else ...
	else {
		return WHITE_COLOR;
	}
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
ColorRGB World::rayTracer(const int& currentRow, const int& currentColumn, const double& xOffset, const double& yOffset)
{
	// First Ray: From Camera out into the world ...
	Point3D firstRayStart;
	Vec3D firstRayDirection;
	camera.getRay(currentRow, currentColumn, xOffset, yOffset, firstRayStart, firstRayDirection);
	Ray3D firstRay{ firstRayStart, firstRayDirection };

	// Run the recursive ray tracer with the first ray
	ColorRGB pixelColor = rayTracerHelper(firstRay, 10);
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
	rays_shot = 0;
	std::cout << "Total Scene Objects: " << sceneObjects.size() << std::endl;

	int rows = camera.getViewWindowRows();
	int cols = camera.getViewWindowCols();

	Image rm{ rows, cols };

	int progressBlock = (rows * cols) / 100;
	size_t blockCount = 0;

	camera.ready();

	// Record Start time
	auto start_time = std::chrono::high_resolution_clock::now();

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
				colors.push_back(rayTracer(i, j, o.first, o.second));
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
	auto render_finish_time = clock();
	double total_render_seconds = ((double)(render_finish_time)) / ((double)(CLOCKS_PER_SEC));
	std::cout << "Total Render Time: " << total_render_seconds << " seconds." << std::endl << std::endl;
	std::cout << "Rays Shot: " << rays_shot << std::endl;

	// Return rendered image
	return rm;
}