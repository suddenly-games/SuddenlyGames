#include "VoxelTriangulation.h"

namespace Engine
{
	Vector3 VoxelTriangulation::Axis[6] = {};
	VoxelTriangulation::TransformationVector VoxelTriangulation::Transformations = TransformationVector();
	VoxelTriangulation::IdentityVector VoxelTriangulation::TriangulationIdentities = IdentityVector();
	VoxelTriangulation::PermutationVector VoxelTriangulation::Permutations = PermutationVector();

	unsigned char VoxelTriangulation::GetPermutation() const
	{
		/*
			87654321
			HGFEDCBA

			00000001 | A - ( -1,  1,  1 )
			00000010 | B - (  1,  1,  1 )
			00000100 | C - (  1, -1,  1 )
			00001000 | D - ( -1, -1,  1 )
			00010000 | E - ( -1, -1, -1 )
			00100000 | F - (  1, -1, -1 )
			01000000 | G - (  1,  1, -1 )
			10000000 | H - ( -1,  1, -1 )
		*/

		unsigned char occupied[8] = {
			unsigned char(unsigned char(Occupancy[1][1][0] > AirThreshold) << 0),
			unsigned char(unsigned char(Occupancy[0][1][0] > AirThreshold) << 1),
			unsigned char(unsigned char(Occupancy[0][0][0] > AirThreshold) << 2),
			unsigned char(unsigned char(Occupancy[1][0][0] > AirThreshold) << 3),
			unsigned char(unsigned char(Occupancy[1][1][1] > AirThreshold) << 4),
			unsigned char(unsigned char(Occupancy[0][1][1] > AirThreshold) << 5),
			unsigned char(unsigned char(Occupancy[0][0][1] > AirThreshold) << 6),
			unsigned char(unsigned char(Occupancy[1][0][1] > AirThreshold) << 7),
		};

		return occupied[7] | occupied[6] | occupied[5] | occupied[4] | occupied[3] | occupied[2] | occupied[1] | occupied[0];
	}
	
	Vector3 VoxelTriangulation::GetPoint(const Vector3& point, const TransformationData& transformation) const
	{
		return point.X * Axis[transformation.RightAxis] + point.Y * Axis[transformation.UpAxis] + point.Z * Axis[transformation.FrontAxis];
	}

	void VoxelTriangulation::Triangulate()
	{
		unsigned char permutation = GetPermutation();

		if (permutation == 0 || permutation == 0xFF)
			return;

		Permutation data = Permutations[permutation];
		Identity& identity = TriangulationIdentities[data.Identity];
		TransformationData& transformation = Transformations[data.Transformation];

		for (int i = 0; i < int(identity.Faces.size()); ++i)
		{
			Face& face = identity.Faces[i];

			Vector3 point1 = 0.5f * Interpolate(GetPoint(face.Point1, transformation)) + Center;
			Vector3 point2 = 0.5f * Interpolate(GetPoint(face.Point2, transformation)) + Center;
			Vector3 point3 = 0.5f * Interpolate(GetPoint(face.Point3, transformation)) + Center;

			if ((point2 - point1).Cross(point3 - point1).SquareLength() > 0.0000001f)
			{
				if (data.Flipped)
					PushTriangle(point1, point3, point2);
				else
					PushTriangle(point1, point2, point3);
			}
		}
	}

	Vector3 VoxelTriangulation::Interpolate(const Vector3& point) const
	{
		Vector3 result = point;

		if (point.X == 0)
		{
			if (point.Y > 0)
			{
				if (point.Z > 0)
					result.X = LerpEdge(Occupancy[0][1][1], Occupancy[1][1][1]);
				else
					result.X = LerpEdge(Occupancy[0][1][0], Occupancy[1][1][0]);
			}
			else
			{
				if (point.Z > 0)
					result.X = LerpEdge(Occupancy[0][0][1], Occupancy[1][0][1]);
				else
					result.X = LerpEdge(Occupancy[0][0][0], Occupancy[1][0][0]);
			}
		}
		else if (point.Y == 0)
		{
			if (point.X > 0)
			{
				if (point.Z > 0)
					result.Y = LerpEdge(Occupancy[1][0][1], Occupancy[1][1][1]);
				else
					result.Y = LerpEdge(Occupancy[1][0][0], Occupancy[1][1][0]);
			}
			else
			{
				if (point.Z > 0)
					result.Y = LerpEdge(Occupancy[0][0][1], Occupancy[0][1][1]);
				else
					result.Y = LerpEdge(Occupancy[0][0][0], Occupancy[0][1][0]);
			}
		}
		else if (point.Z == 0)
		{
			if (point.X > 0)
			{
				if (point.Y > 0)
					result.Z = LerpEdge(Occupancy[1][1][0], Occupancy[1][1][1]);
				else
					result.Z = LerpEdge(Occupancy[1][0][0], Occupancy[1][0][1]);
			}
			else
			{
				if (point.Y > 0)
					result.Z = LerpEdge(Occupancy[0][1][0], Occupancy[0][1][1]);
				else
					result.Z = LerpEdge(Occupancy[0][0][0], Occupancy[0][0][1]);
			}
		}
	
		return result;
	}

	float VoxelTriangulation::LerpEdge(float weight1, float weight2) const
	{
		if (weight2 == 0)
			return -2 * (0.5f - weight1);
		else
			return 2 * (0.5f - weight2);
	}

	void VoxelTriangulation::Initialize()
	{
		InitializeTransformations();
		InitializeIdentities();
		InitializePermutations();
	}
}