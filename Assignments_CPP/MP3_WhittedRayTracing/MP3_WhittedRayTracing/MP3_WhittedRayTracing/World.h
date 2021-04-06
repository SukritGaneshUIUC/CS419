#pragma once
#include <vector>
#include <utility>
#include <memory>
#include <cassert>
#include <ctime>
#include <chrono>

#include "Object.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"
#include "Vec3D.h"
#include "Ray3D.h"
#include "Image.h"
#include "Camera.h"

#include "PointLightSource.h"
#include "AreaLightSource.h"

enum class RenderOption { ANTI_ALIASING, BVH, TRIANGLE_MESH };

const Point3D DEFAULT_VIEW_WINDOW[4]{ Point3D({-8, 4.5, -4.5}), Point3D({8, 4.5, -4.5}), Point3D({8, -4.5, -4.5}), Point3D({-8, -4.5, -4.5}) };

const double MAX_T = 100000;

class World
{
private:
	std::vector<std::shared_ptr<Object>> sceneObjects;
	std::shared_ptr<AreaLightSource> lightSource;
	std::vector<RenderOption> renderOptions;
	Image backgroundImage;
	Camera camera;
	ColorRGB ambientLight;

	size_t rays_shot;
public:
	World();
	~World();

	const std::vector<std::shared_ptr<Object>>& getSceneObjects() const;
	const std::shared_ptr<AreaLightSource> getLightSource() const;
	const std::vector<RenderOption>& getRenderOptions() const;
	const Image& getBackgroundImage();
	const Camera& getCamera() const;
	Camera& getCamera();
	const ColorRGB& getAmbientLight();

	bool OPT_ANTI_ALIASING() const;
	bool OPT_TRIANGLE_MESH() const;

	void addSceneObject(std::shared_ptr<Object> sceneObject);
	void addLightSource(std::shared_ptr<AreaLightSource> lightSource);
	void addRenderOption(const RenderOption& renderOption);
	void setBackgroundImage(Image&& image);
	void setCamera(const Camera& camera);
	void setAmbientLight(const ColorRGB& ambientLight);

	// Ray Tracing Main Methods
	bool shootRay(const Ray3D& firstRay, HitRecord& hitRecord);
	void blinnPhongShading(const Ray3D& ray, const HitRecord& hitRecord, ColorRGB& pixelColor);

	ColorRGB rayTracerHelper(const Ray3D& currRay, double depth);
	ColorRGB rayTracer(const int& currentRow, const int& currentColumn, const double& xOffset, const double& yOffset);
	Image render();
};
