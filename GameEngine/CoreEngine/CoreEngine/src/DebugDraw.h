#pragma once

#include <memory>

#include "Object.h"
#include "Vector3.h"
#include "RGBA.h"
#include "Matrix3.h"

namespace Engine
{
	class Transform;
}

namespace GraphicsEngine
{
	class Model;
	class Material;
	class ModelAsset;
	class Scene;
}

class DebugDraw : public Engine::Object
{
public:
	void Initialize();

	void Update(float) {}

	std::weak_ptr<Engine::ModelAsset> LineAsset;
	std::weak_ptr<Engine::ModelAsset> TriangleAsset;
	std::weak_ptr<GraphicsEngine::Material> MaterialProperties;

	std::shared_ptr<GraphicsEngine::Scene> GetScene();
	void Reset();
	void Clear();
	void DrawLine(const Vector3& point1, const Vector3& point2, const RGBA& color = 0xFFFFFFFF, float thickness = 0.1f, bool glows = false);
	void DrawTriangle(const Vector3& point1, const Vector3& point2, const Vector3& point3, const RGBA& color = 0xFFFFFFFF, float thickness = 0.1f, bool glows = false);

private:
	typedef std::vector<std::weak_ptr<Engine::Transform>> ObjectVector;

	std::weak_ptr<GraphicsEngine::Scene> DebugScene;

	ObjectVector AvailableDebugObjects;
	ObjectVector DebugObjects;
	
	std::shared_ptr<Engine::Transform> NewObject(const Matrix3& transformation);
	void PlaceObject(const Matrix3& transformation, const RGBA& color, float thickness, bool glows, const std::shared_ptr<Engine::ModelAsset>& asset);

	Instantiable;

	Inherits_Class(Object);

	Reflected(DebugDraw);
};

namespace Engine
{
	//Class_Inherits(DebugDraw, Engine::Object);
}