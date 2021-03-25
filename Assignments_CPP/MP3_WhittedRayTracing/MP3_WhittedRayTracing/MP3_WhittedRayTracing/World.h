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
#include "BVHNode.h"

#include "PointLightSource.h"
#include "TriangleMesh.h"

enum class RenderOption { ANTI_ALIASING, BVH, TRIANGLE_MESH };

const Point3D DEFAULT_VIEW_WINDOW[4]{ Point3D({-8, 4.5, -4.5}), Point3D({8, 4.5, -4.5}), Point3D({8, -4.5, -4.5}), Point3D({-8, -4.5, -4.5}) };

const double MAX_T = 100000;

class World
{
private:
	std::vector<std::shared_ptr<Object>> sceneObjects;
	std::vector<std::shared_ptr<Object>> lightSources;
	std::shared_ptr<TriangleMesh> triangleMesh;
	std::vector<RenderOption> renderOptions;
	Image backgroundImage;
	Camera camera;
	ColorRGB ambientLight;

	BVHNode root;

public:
	World();
	~World();

	const std::vector<std::shared_ptr<Object>>& getSceneObjects() const;
	const std::vector<std::shared_ptr<Object>>& getLightSources() const;
	const std::shared_ptr<TriangleMesh>& getTriangleMesh() const;
	const std::vector<RenderOption>& getRenderOptions() const;
	const Image& getBackgroundImage();
	const Camera& getCamera() const;
	Camera& getCamera();
	const ColorRGB& getAmbientLight();

	bool OPT_ANTI_ALIASING() const;
	bool OPT_BVH() const;
	bool OPT_TRIANGLE_MESH() const;

	void addSceneObject(std::shared_ptr<Object> sceneObject);
	void addLightSource(std::shared_ptr<Object> lightSource);
	void setTriangleMesh(const std::shared_ptr<TriangleMesh>& triangleMesh);
	void addRenderOption(const RenderOption& renderOption);
	void setBackgroundImage(Image&& image);
	void setCamera(const Camera& camera);
	void setAmbientLight(const ColorRGB& ambientLight);

	// Ray Tracing Helper Methods
	AABB3D surroundingBox(const AABB3D& box0, const AABB3D& box1) const;
	bool surroundingBox(const std::vector<std::shared_ptr<Object>>& objects, AABB3D& bb) const;

	// Ray Tracing Main Methods
	bool shootPrimaryRay(const int& currentRow, const int& currentColumn, const double& xOffset, const double& yOffset, Ray3D& firstRay, HitRecord& hitRecord);
	void determineColor(const int& currentRow, const int& currentColumn, bool intersected, const Ray3D& firstRay, HitRecord& hitRecord, ColorRGB& pixelColor);

	ColorRGB rayTrace(const int& currentRow, const int& currentColumn, const double& xOffset, const double& yOffset);
	Image render();
};
