#pragma once
#include <vector>
#include <utility>
#include <memory>
#include <cassert>

#include "Object.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"
#include "Vec3D.h"
#include "Ray3D.h"
#include "Image.h"
#include "Camera.h"
#include "BVHNode.h"

#include "PointLightSource.h"
#include "TriangleMesh.h"

enum class RenderOption { ANTI_ALIASING, BVH, TRIANGLE_MESH };

const Point3D DEFAULT_VIEW_WINDOW[4]{ Point3D({-8, 4.5, -4.5}), Point3D({8, 4.5, -4.5}), Point3D({8, -4.5, -4.5}), Point3D({-8, -4.5, -4.5}) };

const double MAX_T = 1000;

class World
{
private:
	std::vector<std::shared_ptr<SceneObject>> sceneObjects;
	std::vector<std::shared_ptr<LightSource>> lightSources;
	TriangleMesh triangleMesh;
	std::vector<RenderOption> renderOptions;
	Image backgroundImage;
	Camera camera;
	ColorRGB ambientLight;

	BVHNode root;

public:
	World();
	~World();

	const std::vector<std::shared_ptr<SceneObject>>& getSceneObjects() const;
	const std::vector<std::shared_ptr<LightSource>>& getLightSource() const;
	const std::vector<RenderOption>& getRenderOptions() const;
	const Image& getBackgroundImage();
	const Camera& getCamera() const;
	Camera& getCamera();
	const ColorRGB& getAmbientLight();
	bool antiAliasing() const;
	bool BVH() const;

	void addSceneObject(std::shared_ptr<SceneObject> sceneObject);
	void addLightSource(std::shared_ptr<LightSource> lightSource);
	void addRenderOption(const RenderOption& renderOption);
	void setBackgroundImage(Image&& image);
	void setCamera(const Camera& camera);
	void setAmbientLight(const ColorRGB& ambientLight);

	// Ray Tracing Helper Methods
	AABB3D surroundingBox(const AABB3D& box0, const AABB3D& box1) const;
	bool surroundingBox(const std::vector<std::shared_ptr<Object>>& objects, AABB3D& bb) const;

	// Ray Tracing Main Methods
	bool bvhIntersectionHelper(const BVHNode& currNode, const Ray3D& firstRay, std::vector<std::shared_ptr<SceneObject>>& intersectedObjects, std::vector<Point3D>& intersectionPoints);
	bool bvhIntersection(const Ray3D& firstRay, std::shared_ptr<SceneObject>& intersectedObject, Point3D& intersectionPoint);
	bool shootPrimaryRay(const int& currentRow, const int& currentColumn, const double& xOffset, const double& yOffset, Point3D& firstRayStart, std::shared_ptr<SceneObject>& intersectedObject, Point3D& intersectionPoint);
	void determineColor(const int& currentRow, const int& currentColumn, bool intersected, const Point3D& firstRayStart, const std::shared_ptr<SceneObject>& intersectedObject, const Point3D& intersectionPoint, ColorRGB& pixelColor);

	ColorRGB rayTrace(const int& currentRow, const int& currentColumn, const double& xOffset, const double& yOffset);
	Image render();
};

//// Ray Tracing Main Methods
//bool shootPrimaryRay(const size_t& currentRow, const size_t& currentColumn, const double& xOffset, const double& yOffset, Ray3D& firstRay, size_t& intersectedObjectIdx, double& intT);
//void determineColor(const size_t& currentRow, const size_t& currentColumn, bool intersected, const Ray3D& firstRay, const size_t& intersectedObjectIdx, const double& intersectionT, ColorRGB& pixelColor);


