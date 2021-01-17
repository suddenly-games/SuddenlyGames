#pragma once

#include <vector>
#include <functional>

#include "Vector3.h"

namespace Engine
{
	class VoxelTriangulation
	{
	private:
		struct TransformationData
		{
			int RightAxis = -1;
			int UpAxis = -1;
			int FrontAxis = -1;

			TransformationData(int right = -1, int up = -1, int front = -1) : RightAxis(right), UpAxis(up), FrontAxis(front) {}
		};

	public:
		typedef std::function<void(const Vector3& corner1, const Vector3& corner2, const Vector3& corner3)> TriangulationCallback;

		static const int Left = 0;
		static const int Right = 1;
		static const int Bottom = 0;
		static const int Top = 1;
		static const int Front = 0;
		static const int Back = 1;

		float AirThreshold = 0.001f;
		float Occupancy[2][2][2] = {};
		TriangulationCallback PushTriangle;
		Vector3 Center;

		void Triangulate();
		unsigned char GetPermutation() const;
		Vector3 GetPoint(const Vector3& point, const TransformationData& transformation) const;
		Vector3 Interpolate(const Vector3& point) const;
		float LerpEdge(float weight1, float weight2) const;

		static void Initialize();
		static void InitializeTransformations();
		static void InitializeIdentities();
		static void InitializePermutations();

	private:
		struct TransformationEnum
		{
			enum Transformation
			{
				Euler___0P___0Y___0R,
				Euler___0P___0Y__90R,
				Euler___0P___0Y_180R,
				Euler___0P___0Y_270R,

				Euler___0P__90Y___0R,
				Euler___0P__90Y__90R,
				Euler___0P__90Y_180R,
				Euler___0P__90Y_270R,

				Euler___0P_180Y___0R,
				Euler___0P_180Y__90R,
				Euler___0P_180Y_180R,
				Euler___0P_180Y_270R,

				Euler___0P_270Y___0R,
				Euler___0P_270Y__90R,
				Euler___0P_270Y_180R,
				Euler___0P_270Y_270R,

				Euler__90P___0Y___0R,
				Euler__90P___0Y__90R,
				Euler__90P___0Y_180R,
				Euler__90P___0Y_270R,

				Euler_270P___0Y___0R,
				Euler_270P___0Y__90R,
				Euler_270P___0Y_180R,
				Euler_270P___0Y_270R,
			};
		};

		struct Face
		{
			Vector3 Point1;
			Vector3 Point2;
			Vector3 Point3;

			Face() {}
			Face(const Vector3& point1, const Vector3& point2, const Vector3& point3) : Point1(point1), Point2(point2), Point3(point3) {}
		};

		typedef std::vector<Face> FaceVector;

		struct Identity
		{
			FaceVector Faces;
		};

		struct Permutation
		{
			int Identity = -1;
			int Transformation = -1;
			bool Flipped = false;

			Permutation(int identity, int transformation, bool flipped = false) : Identity(identity), Transformation(transformation), Flipped(flipped) {}
		};

		typedef std::vector<Identity> IdentityVector;
		typedef std::vector<TransformationData> TransformationVector;
		typedef TransformationEnum::Transformation Transformation;
		typedef std::vector<Permutation> PermutationVector;

		static Vector3 Axis[6];
		static TransformationVector Transformations;
		static IdentityVector TriangulationIdentities;
		static PermutationVector Permutations;
	};
}