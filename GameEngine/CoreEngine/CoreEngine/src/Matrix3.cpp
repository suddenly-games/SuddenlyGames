/*
Matrix.Cpp
Author: 
9/16/16

contains implimentation of Matrix class functions
*/

#include <iostream>

#include "Matrix3.h"

extern "C" {
#include <math.h>
}

Matrix3::Matrix3()
{
	Identity();
}

Matrix3::Matrix3(float x, float y, float z)
{
	Translate(x, y, z);
}

Matrix3::Matrix3(const Vector3& vector)
{
	Translate(vector);
}

Matrix3::Matrix3(const Vector3& position, const Vector3& right, const Vector3& up, const Vector3& front)
{
	SetVectors(position, right, up, front);
}

Matrix3::Matrix3(const Vector3& position, const Vector3& direction, const Vector3& globalUp)
{
	Face(position, direction, globalUp);
}

Matrix3& Matrix3::Identity()
{
	for (int i = 0; i < 16; i++)
		Data[i % 4][i / 4] = i % 4 == i / 4;

	return *this;
}

Matrix3& Matrix3::Transpose()
{
	for (int x = 1; x < 4; ++x)
	{
		for (int y = 0; y < x; ++y)
		{
			float temp = Data[x][y];

			Data[x][y] = Data[y][x];
			Data[y][x] = temp;
		}
	}

	return *this;
}

Matrix3& Matrix3::SetVectors(const Vector3& position, const Vector3& right, const Vector3& up, const Vector3& front)
{
	Translate(position);

	Data[0][0] = right.X;
	Data[1][0] = right.Y;
	Data[2][0] = right.Z;

	Data[0][1] = up.X;
	Data[1][1] = up.Y;
	Data[2][1] = up.Z;

	Data[0][2] = front.X;
	Data[1][2] = front.Y;
	Data[2][2] = front.Z;

	return *this;
}

Matrix3& Matrix3::SetRight(const Vector3& vector)
{
	Data[0][0] = vector.X;
	Data[1][0] = vector.Y;
	Data[2][0] = vector.Z;

	return *this;
}

Matrix3& Matrix3::SetUp(const Vector3& vector)
{
	Data[0][1] = vector.X;
	Data[1][1] = vector.Y;
	Data[2][1] = vector.Z;

	return *this;
}

Matrix3& Matrix3::SetFront(const Vector3& vector)
{
	Data[0][2] = vector.X;
	Data[1][2] = vector.Y;
	Data[2][2] = vector.Z;

	return *this;
}

Matrix3& Matrix3::SetTranslation(const Vector3& vector)
{
	Data[0][3] = vector.X;
	Data[1][3] = vector.Y;
	Data[2][3] = vector.Z;

	return *this;
}

Matrix3& Matrix3::Translate(float x, float y, float z)
{
	Identity();

	Data[0][3] = x;
	Data[1][3] = y;
	Data[2][3] = z;

	return *this;
}

Matrix3& Matrix3::Translate(const Vector3& vector)
{
	return Translate(vector.X, vector.Y, vector.Z);
}

Matrix3& Matrix3::Scale(float x, float y, float z)
{
	Identity();

	Data[0][0] = x;
	Data[1][1] = y;
	Data[2][2] = z;

	return *this;
}

Matrix3& Matrix3::Scale(const Vector3& vector)
{
	return Scale(vector.X, vector.Y, vector.Z);
}

Matrix3& Matrix3::RotateAxis(const Vector3& vector, float theta)
{
	Vector3 normalized = vector;

	normalized.Normalize();
	float axis[3] = { normalized.X, normalized.Y, normalized.Z };

	Identity();

	for (int x = 0; x < 3; ++x)
		for (int y = 0; y < 3; ++y)
			Data[y][x] = axis[x] * axis[y];

	float cosine = cosf(theta);

	*this *= 1 - cosine;

	Matrix3 scale;

	scale.Scale(cosine, cosine, cosine);

	*this += scale;

	Matrix3 cross(
		Vector3(),
		Vector3(0, -normalized.Z, normalized.Y),
		Vector3(normalized.Z, 0, -normalized.X),
		Vector3(-normalized.Y, normalized.X, 0)
	);

	cross *= sinf(theta);

	*this += cross;

	return *this;
}

Matrix3& Matrix3::RotatePitch(float theta)
{
	Identity();

	float sine = sinf(theta);
	float cosine = cosf(theta);

	Data[1][1] = cosine;
	Data[2][1] = sine;

	Data[1][2] = -sine;
	Data[2][2] = cosine;

	return *this;
}

Matrix3& Matrix3::RotateYaw(float theta)
{
	Identity();

	float sine = sinf(theta);
	float cosine = cosf(theta);

	Data[0][0] = cosine;
	Data[2][0] = -sine;

	Data[0][2] = sine;
	Data[2][2] = cosine;

	return *this;
}

Matrix3& Matrix3::RotateRoll(float theta)
{
	Identity();

	float sine = sinf(theta);
	float cosine = cosf(theta);

	Data[0][0] = cosine;
	Data[1][0] = sine;

	Data[0][1] = -sine;
	Data[1][1] = cosine;

	return *this;
}

Matrix3& Matrix3::RotateEulerAngles(float pitch, float yaw, float roll)
{
	*this = Matrix3().RotateRoll(roll) * Matrix3().RotateYaw(yaw) * Matrix3().RotatePitch(pitch);

	return *this;
}

Matrix3& Matrix3::RotateEulerAnglesYaw(float yaw, float pitch, float roll)
{
	*this = Matrix3().RotateRoll(roll) * Matrix3().RotatePitch(pitch) * Matrix3().RotateYaw(yaw);

	return *this;
}

float Matrix3::Det(int y1, int y2, int x1, int x2) const
{
	return Data[y1][x1] * Data[y2][x2] - Data[y1][x2] * Data[y2][x1];
}

Matrix3& Matrix3::Inverse()
{
	float determinant = Det();

	float inverseData[3][3] = {};

	inverseData[0][0] = Det(1, 2, 1, 2);
	inverseData[1][0] = -Det(1, 2, 0, 2);
	inverseData[2][0] = Det(1, 2, 0, 1);

	inverseData[0][1] = -Det(0, 2, 1, 2);
	inverseData[1][1] = Det(0, 2, 0, 2);
	inverseData[2][1] = -Det(0, 2, 0, 1);

	inverseData[0][2] = Det(0, 1, 1, 2);
	inverseData[1][2] = -Det(0, 1, 0, 2);
	inverseData[2][2] = Det(0, 1, 0, 1);

	for (int x = 0; x < 3; ++x)
		for (int y = 0; y < 3; ++y)
			Data[y][x] = inverseData[y][x];

	Vector3 vector = Translation();

	*this *= 1 / determinant;

	Data[0][3] = 0;
	Data[1][3] = 0;
	Data[2][3] = 0;
	Data[3][3] = 1;

	vector = *this * vector;

	Data[0][3] = -vector.X;
	Data[1][3] = -vector.Y;
	Data[2][3] = -vector.Z;

	return *this;
}

Matrix3& Matrix3::Invert(const Matrix3& other)
{
	*this = other;

	Inverse();

	return *this;
}

Matrix3& Matrix3::Projection(float distance, float near, float far, float width, float height)
{
	Scale(2 * distance / width, 2 * distance / height, (near + far) / (near - far));

	Data[3][2] = -1;
	Data[2][3] = 2 * near * far / (near - far);
	Data[3][3] = 0;

	return *this;
}

Matrix3& Matrix3::ExtractRotation(const Matrix3& matrix, const Vector3& newTranslation)
{
	return SetVectors(newTranslation, matrix.RightVector().Normalize(), matrix.UpVector().Normalize(), matrix.FrontVector().Normalize());
}

Matrix3& Matrix3::TransformAround(const Vector3& point)
{
	Vector3 translationVector = (*this * -point) + point;

	Data[0][3] = translationVector.X;
	Data[1][3] = translationVector.Y;
	Data[2][3] = translationVector.Z;

	return *this;
}

Matrix3& Matrix3::Face(const Vector3& position, const Vector3& direction, const Vector3& globalUp)
{
	Vector3 right = direction.Cross(globalUp).Normalize();
	Vector3 up = right.Cross(direction);

	return SetVectors(position, right, up, -direction.Unit());
}

//returns the right vector of a transformation matrix. mostly useful for rotations.
Vector3 Matrix3::RightVector() const
{
	return Vector3(Data[0][0], Data[1][0], Data[2][0]);
}

//returns the up vector of a transformation matrix. mostly useful for rotations.
Vector3 Matrix3::UpVector() const
{
	return Vector3(Data[0][1], Data[1][1], Data[2][1]);
}

//returns the front vector of a transformation matrix. mostly useful for rotations.
Vector3 Matrix3::FrontVector() const
{
	return Vector3(Data[0][2], Data[1][2], Data[2][2]);
}

// returns the translation of the transformation matrix as a point
Vector3 Matrix3::Translation() const
{
	return Vector3(Data[0][3], Data[1][3], Data[2][3], 1);
}

Vector3 Matrix3::ExtractScale() const
{
	return Vector3(RightVector().Length(), UpVector().Length(), FrontVector().Length());
}

float Matrix3::Det() const
{
	return Data[0][0] * Det(1, 2, 1, 2) - Data[0][1] * Det(1, 2, 0, 2) + Data[0][2] * Det(1, 2, 0, 1);
}

Matrix3 Matrix3::operator+(const Matrix3 &other) const
{
	Matrix3 result;

	for (int x = 0; x < 3; ++x)
		for (int y = 0; y < 3; ++y)
			result.Data[y][x] = Data[y][x] + other.Data[y][x];

	return result;
}

Matrix3 Matrix3::operator-(const Matrix3 &other) const
{
	Matrix3 result;

	for (int x = 0; x < 3; ++x)
		for (int y = 0; y < 3; ++y)
			result.Data[y][x] = Data[y][x] - other.Data[y][x];

	return result;
}

Matrix3 Matrix3::operator-() const
{
	Matrix3 result;

	for (int x = 0; x < 3; ++x)
		for (int y = 0; y < 3; ++y)
			result.Data[y][x] = -Data[y][x];

	return result;
}

// scalar multiply
Matrix3 Matrix3::operator*(float scalar) const
{
	Matrix3 result;

	for (int x = 0; x < 3; ++x)
		for (int y = 0; y < 3; ++y)
			result.Data[y][x] = scalar * Data[y][x];

	return result;
}

float Matrix3::ComponentMultiplication(const Matrix3 &other, int y, int x) const
{
	return Data[y][0] * other.Data[0][x] + Data[y][1] * other.Data[1][x] + Data[y][2] * other.Data[2][x] + Data[y][3] * other.Data[3][x];
}

//overloaded multiplication operator
Matrix3 Matrix3::operator*(const Matrix3 &other) const
{
	Matrix3 results;

	// multiply first row
	results.Data[0][0] = ComponentMultiplication(other, 0, 0);
	results.Data[0][1] = ComponentMultiplication(other, 0, 1);
	results.Data[0][2] = ComponentMultiplication(other, 0, 2);
	results.Data[0][3] = ComponentMultiplication(other, 0, 3);

	// multiply second row
	results.Data[1][0] = ComponentMultiplication(other, 1, 0);
	results.Data[1][1] = ComponentMultiplication(other, 1, 1);
	results.Data[1][2] = ComponentMultiplication(other, 1, 2);
	results.Data[1][3] = ComponentMultiplication(other, 1, 3);

	// multiply third row
	results.Data[2][0] = ComponentMultiplication(other, 2, 0);
	results.Data[2][1] = ComponentMultiplication(other, 2, 1);
	results.Data[2][2] = ComponentMultiplication(other, 2, 2);
	results.Data[2][3] = ComponentMultiplication(other, 2, 3);

	// multiply fourth row
	results.Data[3][0] = ComponentMultiplication(other, 3, 0);
	results.Data[3][1] = ComponentMultiplication(other, 3, 1);
	results.Data[3][2] = ComponentMultiplication(other, 3, 2);
	results.Data[3][3] = ComponentMultiplication(other, 3, 3);

	return results;
}

// transform a vector
Vector3 Matrix3::operator*(const Vector3 &other) const
{
	return Vector3(
		other.X * Data[0][0] + other.Y * Data[0][1] + other.Z * Data[0][2] + other.W * Data[0][3],
		other.X * Data[1][0] + other.Y * Data[1][1] + other.Z * Data[1][2] + other.W * Data[1][3],
		other.X * Data[2][0] + other.Y * Data[2][1] + other.Z * Data[2][2] + other.W * Data[2][3],
		other.X * Data[3][0] + other.Y * Data[3][1] + other.Z * Data[3][2] + other.W * Data[3][3]
	);
}

float* Matrix3::operator[](int row)
{
	return Data[row];
}

const float* Matrix3::operator[](int row) const
{
	return Data[row];
}

// assignment
Matrix3& Matrix3::operator=(const Matrix3 &other)
{
	for (int x = 0; x < 4; ++x)
		for (int y = 0; y < 4; ++y)
			Data[y][x] = other.Data[y][x];

	return *this;
}

// addition assignment
Matrix3& Matrix3::operator+=(const Matrix3 &other)
{
	for (int x = 0; x < 3; ++x)
		for (int y = 0; y < 3; ++y)
			Data[y][x] += other.Data[y][x];

	return *this;
}

// subtraction assignment
Matrix3& Matrix3::operator-=(const Matrix3 &other)
{
	for (int x = 0; x < 3; ++x)
		for (int y = 0; y < 3; ++y)
			Data[y][x] -= other.Data[y][x];

	return *this;
}

// transformation assignment
Matrix3& Matrix3::operator*=(const Matrix3 &other)
{
	*this = *this * other;

	return *this;
}

// scalar assignment
Matrix3& Matrix3::operator*=(float scalar)
{
	for (int x = 0; x < 3; ++x)
		for (int y = 0; y < 3; ++y)
			Data[y][x] *= scalar;

	return *this;
}

bool Matrix3::Compare(float x, float y, float epsilon)
{
	return abs(x - y) < epsilon;
}

bool Matrix3::operator==(const Matrix3& other) const
{
	for (int x = 0; x < 4; ++x)
		for (int y = 0; y < 4; ++y)
			if (!Compare(Data[y][x], other.Data[y][x], 1e-4f))
				return false;

	return true;
}

bool Matrix3::operator!=(const Matrix3& other) const
{
	return !(*this == other);
}

// scalar multiply
Matrix3 operator*(float scalar, const Matrix3& matrix)
{
	Matrix3 result;
		
	for (int x = 0; x < 3; ++x)
		for (int y = 0; y < 3; ++y)
			result.Data[y][x] = scalar * matrix.Data[y][x];
		
	return result;
}

Matrix3::operator std::string() const
{
	std::stringstream out;

	out << *this;

	return out.str();
}
	
// stream output
std::ostream& operator<<(std::ostream &out, const Matrix3 &matrix)
{
	// put the formatted data into the provided output stream
	out << "[ " << matrix.Data[0][0] << ", " << matrix.Data[0][1] << ", " << matrix.Data[0][2] << ", " << matrix.Data[0][3] << " ]\n"
		<< "[ " << matrix.Data[1][0] << ", " << matrix.Data[1][1] << ", " << matrix.Data[1][2] << ", " << matrix.Data[1][3] << " ]\n"
		<< "[ " << matrix.Data[2][0] << ", " << matrix.Data[2][1] << ", " << matrix.Data[2][2] << ", " << matrix.Data[2][3] << " ]\n"
		<< "[ " << matrix.Data[3][0] << ", " << matrix.Data[3][1] << ", " << matrix.Data[3][2] << ", " << matrix.Data[3][3] << " ]" << std::endl;
		
	return out;
}
