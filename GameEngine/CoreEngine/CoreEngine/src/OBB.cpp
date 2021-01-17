#include "OBB.h"

#include <numeric>
#include <algorithm>
#include "PCA.h"
#include "MeshLoader.h"
#include "ModelAsset.h"
#include "MeshData.h"

OBB OBB::Compute(const MeshData::VertexVector& vertices)
{
	Matrix3 covariance = PCA::ComputeCovariance(vertices);

	PCA eigenData = PCA::Compute(covariance);

	eigenData.Axis1.Normalize();
	eigenData.Axis2.Normalize();
	eigenData.Axis3.Normalize();

	constexpr float minValue = std::numeric_limits<float>::lowest();
	constexpr float maxValue = std::numeric_limits<float>::max();

	Vector3 min = Vector3(minValue, minValue, minValue);
	Vector3 max = Vector3(maxValue, maxValue, maxValue);

	for (int i = 0; i < int(vertices.size()); ++i)
	{
		Vector3 values(
			Vector3(vertices[i].Position) * eigenData.Axis1,
			Vector3(vertices[i].Position) * eigenData.Axis2,
			Vector3(vertices[i].Position) * eigenData.Axis3
		);
		
		min.Set(
			std::min(min.X, values.X),
			std::min(min.Y, values.Y),
			std::min(min.Z, values.Z)
		);
		
		max.Set(
			std::max(max.X, values.X),
			std::max(max.Y, values.Y),
			std::max(max.Z, values.Z)
		);
	}

	Vector3 middle = 0.5f * (min + max);

	OBB boundingBox;

	boundingBox.Center = middle.X * eigenData.Axis1 + middle.Y * eigenData.Axis2 + middle.Z * eigenData.Axis3;
	boundingBox.Size = max - min;
	boundingBox.Transformation = Matrix3(boundingBox.Center, eigenData.Axis1, eigenData.Axis2, eigenData.Axis3) * Matrix3().Scale(boundingBox.Size);

	return boundingBox;
}

OBB OBB::Compute(const std::shared_ptr<Engine::ModelAsset>& model)
{
	int meshID = model->GetMeshID();

	return Compute(MeshLoader::GetMeshData(meshID)->VertexBuffer);
}