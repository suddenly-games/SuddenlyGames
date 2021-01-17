#include "PCA.h"

#include "MeshData.h"

PCA PCA::Compute(const Matrix3& covariance, int maxIterations, float epsilon)
{
	Matrix3 eigenVectors;
	Matrix3 diagonal = covariance;

	for (int i = 0; i < maxIterations; ++i)
	{
		Matrix3 rotation = ComputeJacobiRotation(diagonal);

		eigenVectors = eigenVectors * rotation;
		diagonal = diagonal * rotation;

		rotation.Data[0][1] *= -1;
		rotation.Data[0][2] *= -1;
		rotation.Data[1][2] *= -1;

		rotation.Data[1][0] *= -1;
		rotation.Data[2][0] *= -1;
		rotation.Data[2][1] *= -1;

		diagonal = rotation * diagonal;

		if (diagonal.Data[0][1] * diagonal.Data[0][1] + diagonal.Data[0][2] * diagonal.Data[0][2] + diagonal.Data[1][2] * diagonal.Data[1][2] +
			diagonal.Data[1][0] * diagonal.Data[1][0] + diagonal.Data[2][0] * diagonal.Data[2][0] + diagonal.Data[2][1] * diagonal.Data[2][1] < epsilon)
			break;
	}

	PCA results;

	results.Axis1 = eigenVectors.RightVector();
	results.Axis2 = eigenVectors.UpVector();
	results.Axis3 = eigenVectors.FrontVector();
	results.EigenValues.Set(diagonal.Data[0][0], diagonal.Data[1][1], diagonal.Data[2][2]);

	return results;
}

Matrix3 PCA::ComputeCovariance(const MeshData::VertexVector& vertices)
{
	Vector3 mean(0, 0, 0);

	for (int i = 0; i < int(vertices.size()); ++i)
		mean += vertices[i].Position;

	mean *= 1 / float(vertices.size());

	Matrix3 covariance;

	covariance.Data[0][0] = 0;
	covariance.Data[1][1] = 0;
	covariance.Data[2][2] = 0;

	for (int i = 0; i < int(vertices.size()); ++i)
	{
		Vector3 vec = vertices[i].Position - mean;

		for (int j = 0; j < 9; ++j)
			covariance.Data[j / 3][j % 3] += vec[j / 3] * vec[j % 3];
	}

	for (int i = 0; i < 9; ++i)
		covariance.Data[i / 3][i % 3] /= float(vertices.size());

	return covariance;
}

namespace
{
	struct SinCos
	{
		float s, c;
	};

	SinCos computeRotation(float diag1, float diag2, float offDiag)
	{
		SinCos values;

		float b = 0.5f * (diag2 - diag1) / offDiag;

		float tangent = (b >= 0 ? 1 : -1) / (abs(b) + sqrtf(b * b + 1));

		values.c = sqrtf(1 / (tangent * tangent + 1));
		values.s = sqrtf(1 - values.c * values.c);

		if (tangent < 0)
			values.s *= -1;

		return values;
	}
}

Matrix3 PCA::ComputeJacobiRotation(const Matrix3& matrix)
{
	float val1 = abs(matrix.Data[0][1]);
	float val2 = abs(matrix.Data[0][2]);
	float val3 = abs(matrix.Data[1][2]);

	Matrix3 rotation;

	if (val1 > val2&& val1 > val3)
	{
		SinCos values = computeRotation(matrix.Data[0][0], matrix.Data[1][1], matrix.Data[0][1]);

		rotation.Data[0][0] = values.c;
		rotation.Data[1][1] = values.c;
		rotation.Data[0][1] = values.s;
		rotation.Data[1][0] = -values.s;
	}
	else if (val2 > val1&& val2 > val3)
	{
		SinCos values = computeRotation(matrix.Data[0][0], matrix.Data[2][2], matrix.Data[0][2]);

		rotation.Data[0][0] = values.c;
		rotation.Data[2][2] = values.c;
		rotation.Data[0][2] = values.s;
		rotation.Data[2][0] = -values.s;
	}
	else
	{
		SinCos values = computeRotation(matrix.Data[1][1], matrix.Data[2][2], matrix.Data[1][2]);

		rotation.Data[1][1] = values.c;
		rotation.Data[2][2] = values.c;
		rotation.Data[1][2] = values.s;
		rotation.Data[2][1] = -values.s;
	}

	return rotation;
}