#include "DebugDraw.h"

#include "Model.h"
#include "Transform.h"

void DebugDraw::Initialize()
{
	auto debugScene = Engine::Create<GraphicsEngine::Scene>();

	debugScene->SetParent(This.lock());

	DebugScene = debugScene;
}

std::shared_ptr<GraphicsEngine::Scene> DebugDraw::GetScene()
{
	return DebugScene.lock();
}

void DebugDraw::Reset()
{
	for (int i = 0; i < int(DebugObjects.size()); ++i)
	{
		DebugScene.lock()->RemoveObject(DebugObjects[i].lock()->Get<GraphicsEngine::Model>());

		AvailableDebugObjects.push_back(std::weak_ptr<Engine::Transform>());
		AvailableDebugObjects.back() = DebugObjects[i];
	}

	DebugObjects.clear();
}

void DebugDraw::Clear()
{
	Reset();

	AvailableDebugObjects.clear();
}

void DebugDraw::DrawLine(const Vector3& point1, const Vector3& point2, const RGBA& color, float thickness, bool glows)
{

	Vector3 vector = point2 - point1;
	float length = vector.Length();

	vector.Normalize();

	Vector3 up = Vector3(0, 1, 0);

	if (vector.X >= -1e-5f && vector.X <= 1e-5f && vector.Z >= -1e-5f && vector.Z <= 1e-5f)
		up = Vector3(1, 0, 0);

	PlaceObject(
		Matrix3().Translate(0.5f * length * vector) * Matrix3(point1, vector, up) * Matrix3().Scale(thickness, thickness, 0.5f * length),
		color,
		thickness,
		glows,
		LineAsset.lock()
	);
}

void DebugDraw::DrawTriangle(const Vector3& point1, const Vector3& point2, const Vector3& point3, const RGBA& color, float thickness, bool glows)
{
	Vector3 ab = point2 - point1;
	Vector3 ac = point3 - point1;

	Vector3 normal = ac.Cross(ab).Normalize();
	
	float squareDistance = ac.Dot(ac);
	float distance = sqrtf(squareDistance);
	float dot = ab.Dot(ac) / squareDistance;
	float distance1 = dot * distance;
	
	float v2x = ac.X / distance;
	float v2y = ac.Y / distance;
	float v2z = ac.Z / distance;
	
	float v1x = v2y * normal.Z - v2z * normal.Y;
	float v1y = v2z * normal.X - v2x * normal.Z;
	float v1z = v2x * normal.Y - v2y * normal.X;
	
	float dx = ab.X - dot * ac.X;
	float dy = ab.Y - dot * ac.Y;
	float dz = ab.Z - dot * ac.Z;
	
	float height = -(v1x * dx + v1y * dy + v1z * dz);
	
	PlaceObject(
		(
			Matrix3(
				Vector3(0.5f * (point1.X + point2.X) + 0.1f * normal.X, 0.5f * (point1.Y + point2.Y) + 0.1f * normal.Y, 0.5f * (point1.Z + point2.Z) + 0.1f * normal.Z),
				Vector3(normal.X, -v2x, v1x),
				Vector3(normal.Y, -v2y, v1y),
				Vector3(normal.Z, -v2z, v1z)
			) *
			Matrix3().Scale(thickness, 0.5f * distance1, 0.5f * height)
		),
		color,
		thickness,
		glows,
		LineAsset.lock()
	);

	PlaceObject(
		(
			Matrix3(
				Vector3(0.5f * (point2.X + point3.X) + 0.1f * normal.X, 0.5f * (point2.Y + point3.Y) + 0.1f * normal.Y, 0.5f * (point3.Z + point3.Z) + 0.1f * normal.Z),
				Vector3(normal.X, -v1x, -v2x),
				Vector3(normal.Y, -v1y, -v2y),
				Vector3(normal.Z, -v1z, -v2z)
			) *
			Matrix3().Scale(thickness, 0.5f * height, 0.5f * (distance - distance1))
			),
		color,
		thickness,
		glows,
		LineAsset.lock()
	);
}

std::shared_ptr<Engine::Transform> DebugDraw::NewObject(const Matrix3& transformation)
{
	std::shared_ptr<Engine::Transform> object;

	if (AvailableDebugObjects.size() > 0)
	{
		object = AvailableDebugObjects.back().lock();

		AvailableDebugObjects.pop_back();
	}
	else
	{
		object = Engine::Create<Engine::Transform>();

		object->IsStatic = false;

		Engine::Create<GraphicsEngine::Model>()->SetParent(object);
	}

	DebugObjects.push_back(std::weak_ptr<Engine::Transform>());
	DebugObjects.back() = object;

	object->SetParent(This.lock());

	object->Transformation = transformation;

	DebugScene.lock()->AddObject(object->Get<GraphicsEngine::Model>());

	return object;
}

void DebugDraw::PlaceObject(const Matrix3& transformation, const RGBA& color, float thickness, bool glows, const std::shared_ptr<Engine::ModelAsset>& asset)
{
	std::shared_ptr<Engine::Transform> transform = NewObject(transformation);
	std::shared_ptr<GraphicsEngine::Model> model = transform->Get<GraphicsEngine::Model>();

	model->Color = color;

	if (glows)
		model->GlowColor = color;
	else
		model->GlowColor = 0x00000000;

	model->Asset = asset;
	model->MaterialProperties = MaterialProperties;
}