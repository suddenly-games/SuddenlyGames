#include "BoundingSphere.h"

#include <algorithm>
#include <numeric>
#include "PCA.h"
#include "MeshLoader.h"
#include "ModelAsset.h"

BoundingSphere BoundingSphere::ComputeCentroid(const std::shared_ptr<Engine::ModelAsset>& model)
{
	int meshID = model->GetMeshID();

	return ComputeCentroid(MeshLoader::GetMeshData(meshID)->VertexBuffer);
}

BoundingSphere BoundingSphere::ComputeRitter(const std::shared_ptr<Engine::ModelAsset>& model)
{
	int meshID = model->GetMeshID();

	return ComputeRitter(MeshLoader::GetMeshData(meshID)->VertexBuffer);
}

BoundingSphere BoundingSphere::ComputeLarson(const std::shared_ptr<Engine::ModelAsset>& model)
{
	int meshID = model->GetMeshID();

	return ComputeLarson(MeshLoader::GetMeshData(meshID)->VertexBuffer);
}

BoundingSphere BoundingSphere::ComputePCA(const std::shared_ptr<Engine::ModelAsset>& model)
{
	int meshID = model->GetMeshID();

	return ComputePCA(MeshLoader::GetMeshData(meshID)->VertexBuffer);
}

bool BoundingSphere::Contains(const Vector3& point)
{
	return (point - Center).SquareLength() <= Radius * Radius;
}

void BoundingSphere::ExpandByPoint(const Vector3& point)
{
	Vector3 diff = point - Center;
	float length = diff.SquareLength();

	if (length <= Radius * Radius)
		return;

	length = sqrtf(length);
	diff *= 1 / length;

	Center = 0.5f * (Center - Radius * diff + point);
	Radius = 0.5f * (length + Radius);
}

BoundingSphere BoundingSphere::ComputeCentroid(const MeshData::VertexVector& vertices)
{
	if (vertices.size() == 0)
		return BoundingSphere();
	else if (vertices.size() == 1)
		return BoundingSphere(0, vertices[0].Position);

	Vector3 minPoint = vertices[0].Position;
	Vector3 maxPoint = vertices[0].Position;

	for (int i = 1; i < int(vertices.size()); ++i)
	{
		minPoint.X = std::min(minPoint.X, vertices[i].Position.X);
		minPoint.Y = std::min(minPoint.Y, vertices[i].Position.Y);
		minPoint.Z = std::min(minPoint.Z, vertices[i].Position.Z);

		maxPoint.X = std::max(maxPoint.X, vertices[i].Position.X);
		maxPoint.Y = std::max(maxPoint.Y, vertices[i].Position.Y);
		maxPoint.Z = std::max(maxPoint.Z, vertices[i].Position.Z);
	}

	Vector3 center;

	center = 0.5f * (minPoint + maxPoint);
	float maxRadius = 0;

	for (int i = 0; i < int(vertices.size()); ++i)
		maxRadius = std::max(maxRadius, (Vector3(vertices[i].Position) - center).SquareLength());

	return BoundingSphere(sqrtf(maxRadius), center);
}

BoundingSphere BoundingSphere::ComputeRitter(const MeshData::VertexVector& vertices)
{
	if (vertices.size() == 0)
		return BoundingSphere();
	else if (vertices.size() == 1)
		return BoundingSphere(0, vertices[0].Position);

	Vector3 minPoint = vertices[0].Position;
	Vector3 maxPoint = vertices[0].Position;

	int minX = 0;
	int minY = 0;
	int minZ = 0;

	int maxX = 0;
	int maxY = 0;
	int maxZ = 0;

	for (int i = 1; i < int(vertices.size()); ++i)
	{
		if (vertices[i].Position.X < minPoint.X)
		{
			minPoint.X = vertices[i].Position.X;
			minX = i;
		}

		if (vertices[i].Position.Y < minPoint.Y)
		{
			minPoint.Y = vertices[i].Position.Y;
			minY = i;
		}

		if (vertices[i].Position.Z < minPoint.Z)
		{
			minPoint.Z = vertices[i].Position.Z;
			minZ = i;
		}

		if (vertices[i].Position.X > maxPoint.X)
		{
			maxPoint.X = vertices[i].Position.X;
			maxX = i;
		}

		if (vertices[i].Position.Y > maxPoint.Y)
		{
			maxPoint.Y = vertices[i].Position.Y;
			maxY = i;
		}

		if (vertices[i].Position.Z > maxPoint.Z)
		{
			maxPoint.Z = vertices[i].Position.Z;
			maxZ = i;
		}
	}

	float lengthX = (Vector3(vertices[minX].Position) - vertices[maxX].Position).SquareLength();
	float lengthY = (Vector3(vertices[minY].Position) - vertices[maxY].Position).SquareLength();
	float lengthZ = (Vector3(vertices[minZ].Position) - vertices[maxZ].Position).SquareLength();

	Vector3 bound1;
	Vector3 bound2;

	if (lengthX > lengthY&& lengthX > lengthZ)
	{
		bound1 = vertices[minX].Position;
		bound2 = vertices[maxX].Position;
	}
	else if (lengthY > lengthZ)
	{
		bound1 = vertices[minY].Position;
		bound2 = vertices[maxY].Position;
	}
	else
	{
		bound1 = vertices[minZ].Position;
		bound2 = vertices[maxZ].Position;
	}

	BoundingSphere sphere(0.5f * (bound1 - bound2).Length(), 0.5f * (bound1 + bound2));

	for (int i = 0; i < int(vertices.size()); ++i)
		sphere.ExpandByPoint(vertices[i].Position);

	return sphere;
}

namespace
{
	inline void trySwap(int* x, int* y)
	{
		if (*x > * y)
			std::swap(*x, *y);
	}

	void fastSort(int* data)
	{
		trySwap(data + 1, data + 2);
		trySwap(data + 0, data + 2);
		trySwap(data + 0, data + 1);
		trySwap(data + 4, data + 5);
		trySwap(data + 3, data + 5);
		trySwap(data + 3, data + 4);
		trySwap(data + 0, data + 3);
		trySwap(data + 1, data + 4);
		trySwap(data + 2, data + 5);
		trySwap(data + 2, data + 4);
		trySwap(data + 1, data + 3);
		trySwap(data + 2, data + 3);
	}

	int removeDuplicates(int* data)
	{
		int next = 0;
		int current = 0;

		while (next < 6)
		{
			if (data[next] != data[current])
			{
				data[current] = data[next];
				
				++current;
			}

			++next;
		}

		return current;
	}
}

BoundingSphere BoundingSphere::ComputeLarson(const MeshData::VertexVector& vertices)
{
	if (vertices.size() == 0)
		return BoundingSphere();
	else if (vertices.size() == 1)
		return BoundingSphere(0, vertices[0].Position);

	if (vertices.size() > 6)
	{
		int extremePoints[6] = { 0, 0, 0, 0, 0, 0 };
		Vector3 min = vertices[0].Position;
		Vector3 max = vertices[0].Position;

		for (int i = 1; i < int(vertices.size()); ++i)
		{
			Vector3 point = vertices[i].Position;

			if (point.X < min.X)
				extremePoints[0] = i;
			else if (point.X > max.X)
				extremePoints[1] = i;

			if (point.Y < min.Y)
				extremePoints[2] = i;
			else if (point.Y > max.Y)
				extremePoints[3] = i;

			if (point.Z < min.Z)
				extremePoints[4] = i;
			else if (point.Z > max.Z)
				extremePoints[5] = i;

			min.Set(
				std::min(min.X, point.X),
				std::min(min.Y, point.Y),
				std::min(min.Z, point.Z)
			);

			max.Set(
				std::max(max.X, point.X),
				std::max(max.Y, point.Y),
				std::max(max.Z, point.Z)
			);
		}

		fastSort(extremePoints);

		int pointCount = removeDuplicates(extremePoints);

		BoundingSphere sphere = ComputeExactSphere(vertices, extremePoints, pointCount);

		for (int i = 0; i < int(vertices.size()); ++i)
			sphere.ExpandByPoint(vertices[i].Position);

		return sphere;
	}
	else
	{
		int extremePoints[6] = { 0, 1, 2, 3, 4, 5 };

		return ComputeExactSphere(vertices, extremePoints, int(vertices.size()));
	}
}

BoundingSphere BoundingSphere::ComputeExactSphere(const MeshData::VertexVector& vertices, int* indices, int indexCount)
{
	int farthest1 = 0;
	int farthest2 = 0;
	float farthestDistance = 0;

	for (int i = 0; i < indexCount; ++i)
	{
		for (int j = 0; j < indexCount; ++j)
		{
			float distance = (Vector3(vertices[indices[i]].Position) - vertices[indices[j]].Position).SquareLength();

			if (distance > farthestDistance)
			{
				farthestDistance = distance;
				farthest1 = i;
				farthest2 = j;
			}
		}
	}

	Vector3 center = 0.5f * (Vector3(vertices[indices[farthest1]].Position) + vertices[indices[farthest2]].Position);
	float radiusSquared = (center + vertices[indices[farthest2]].Position).SquareLength();
	float radius = std::sqrt(radiusSquared);

	while (indexCount > 0)
	{
		int expandingTo = -1;
		float farthest = radiusSquared;

		for (int i = 0; i < indexCount;)
		{
			float distance = (Vector3(vertices[indices[i]].Position) - center).SquareLength();

			if (distance > farthest)
			{
				farthest = distance;
				expandingTo = i;
			}
			else if (distance <= radiusSquared)
			{
				std::swap(indices[i], indices[indexCount - 1]);
				
				--indexCount;
			}
		}

		if (indexCount > 0)
		{
			float distance = std::sqrt(farthest);

			center = 0.5f * (1 - radius / distance) * (vertices[indices[expandingTo]].Position - center);
			radius += 0.5f * (distance - radius);
			radiusSquared = radius * radius;
		}
	}

	return BoundingSphere(radius, center);
}

BoundingSphere BoundingSphere::ComputePCA(const MeshData::VertexVector& vertices)
{
	if (vertices.size() == 0)
		return BoundingSphere();
	else if (vertices.size() == 1)
		return BoundingSphere(0, vertices[0].Position);

	Matrix3 covariance = PCA::ComputeCovariance(vertices);

	PCA eigenData = PCA::Compute(covariance);

	Vector3 axis;
	Vector3 eigenValues = eigenData.EigenValues;

	if (eigenValues.X > eigenValues.Y&& eigenValues.X > eigenValues.Z)
		axis = eigenData.Axis1;
	else if (eigenValues.Y > eigenValues.X && eigenValues.Y > eigenValues.Z)
		axis = eigenData.Axis2;
	else
		axis = eigenData.Axis3;

	float smallestDot = Vector3(vertices[0].Position).Dot(axis);
	float largestDot = smallestDot;

	int i1 = 0;
	int i2 = 0;

	for (int i = 1; i < int(vertices.size()); ++i)
	{
		float dot = Vector3(vertices[i].Position).Dot(axis);

		if (dot < smallestDot)
		{
			smallestDot = dot;
			i1 = i;
		}

		if (dot > largestDot)
		{
			largestDot = dot;
			i2 = i;
		}
	}

	BoundingSphere sphere(0.5f * Vector3(vertices[i1].Position - vertices[i2].Position).Length(), 0.5f * (vertices[i1].Position + vertices[i2].Position));

	for (int i = 0; i < int(vertices.size()); ++i)
		sphere.ExpandByPoint(vertices[i].Position);

	return sphere;
}
