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

#include "PointLightSource.h"

enum class RenderOption { ANTI_ALIASING };

const Point3D DEFAULT_VIEW_WINDOW[4]{ Point3D({-8, 4.5, -4.5}), Point3D({8, 4.5, -4.5}), Point3D({8, -4.5, -4.5}), Point3D({-8, -4.5, -4.5}) };

class World
{
private:
	std::vector<std::shared_ptr<SceneObject>> sceneObjects;
	std::vector<std::shared_ptr<LightSource>> lightSources;
	std::vector<RenderOption> renderOptions;
	Image backgroundImage;
	Camera camera;
	ColorRGB ambientLight;

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

	void addSceneObject(std::shared_ptr<SceneObject> sceneObject);
	void addLightSource(std::shared_ptr<LightSource> lightSource);
	void addRenderOption(const RenderOption& renderOption);
	void setBackgroundImage(Image&& image);
	void setCamera(const Camera& camera);
	void setAmbientLight(const ColorRGB& ambientLight);

	bool antiAliasing() const;

	ColorRGB rayTrace(const int& currentRow, const int& currentColumn, const double& xOffset, const double& yOffset);
	Image render();
};

