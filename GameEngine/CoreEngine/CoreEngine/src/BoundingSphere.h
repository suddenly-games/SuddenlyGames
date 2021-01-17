#pragma once

#include <vector>
#include "Vector3.h"
#include "Matrix3.h"
#include "ObjectReflection.h"
#include "MeshData.h"

namespace Engine
{
	class ModelAsset;
}

class BoundingSphere
{
public:
	typedef std::vector<Vector3> VertexVector;

	float Radius = 0;
	Vector3 Center;

	BoundingSphere(float radius = 0, const Vector3& center = Vector3()) : Radius(radius), Center(center) {}

	bool Contains(const Vector3& point);
	void ExpandByPoint(const Vector3& point);

	static BoundingSphere ComputeCentroid(const MeshData::VertexVector& vertices);
	static BoundingSphere ComputeRitter(const MeshData::VertexVector& vertices);
	static BoundingSphere ComputeLarson(const MeshData::VertexVector& vertices);
	static BoundingSphere ComputePCA(const MeshData::VertexVector& vertices);
	static BoundingSphere ComputeExactSphere(const MeshData::VertexVector& vertices, int* indices, int indexCount);

	static BoundingSphere ComputeCentroid(const std::shared_ptr<Engine::ModelAsset>& model);
	static BoundingSphere ComputeRitter(const std::shared_ptr<Engine::ModelAsset>& model);
	static BoundingSphere ComputeLarson(const std::shared_ptr<Engine::ModelAsset>& model);
	static BoundingSphere ComputePCA(const std::shared_ptr<Engine::ModelAsset>& model);

	Base_Class;

	Reflected_Type(BoundingSphere);
};

namespace Engine
{
	//Class_Inherits(BoundingSphere, CoreType);

	Define_Value_Type(BoundingSphere);
}
