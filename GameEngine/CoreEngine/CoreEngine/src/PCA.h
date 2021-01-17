#pragma once

#include <vector>
#include "Vector3.h"
#include "Matrix3.h"
#include "MeshData.h"

class PCA
{
public:
	typedef std::vector<Vector3> VertexVector;

	Vector3 Axis1;
	Vector3 Axis2;
	Vector3 Axis3;
	Vector3 EigenValues;

	static PCA Compute(const Matrix3& covariance, int maxIterations = 50, float epsilon = 0.0001f);
	static Matrix3 ComputeCovariance(const MeshData::VertexVector& vertices);
	static Matrix3 ComputeJacobiRotation(const Matrix3& matrix);
};