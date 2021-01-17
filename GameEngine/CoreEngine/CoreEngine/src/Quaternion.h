#pragma once

#include "Matrix3.h"

extern "C" {
#include <math.h>
}

#include "ObjectReflection.h"

class Quaternion
{
public:
	float X, Y, Z, W;

	Quaternion(float w = 0, float x = 0, float y = 0, float z = 0) : X(x), Y(y), Z(z), W(w) {}
	Quaternion(const Quaternion& quat) : X(quat.X), Y(quat.Y), Z(quat.Z), W(quat.W) {}
	Quaternion(const Vector3& vec) : X(vec.X), Y(vec.Y), Z(vec.Z), W(vec.W) {}
	Quaternion(const Vector3& axis, float angle)
	{
		*this = sinf(0.5f * angle) * Vector3(axis.X, axis.Y, axis.Z, 0).Normalize() + Vector3(0, 0, 0, cosf(0.5f * angle));
	}
	Quaternion(float w, const Vector3& vector) : X(vector.X), Y(vector.Y), Z(vector.Z), W(w) {}
	Quaternion(const Matrix3& matrix);

	Quaternion& Invert();
	Quaternion Inverse() { return Quaternion(*this).Invert(); }
	Quaternion Slerp(const Quaternion& destination, float t) const;

	Quaternion operator*(const Quaternion& rhs) const;
	//Vector3 operator*(const Vector3& rhs) const;

	Matrix3 Matrix() const;

	Quaternion& Normalize();
	Quaternion Cross(const Quaternion& other) const;
	Quaternion Unit() const;
	float Length() const;
	float SquareLength() const;
	float Dot(const Quaternion& other) const;

	Quaternion operator-() const;
	Quaternion operator+(const Quaternion& other) const;
	Quaternion operator-(const Quaternion& other) const;
	Quaternion operator*(float scalar) const;
	Quaternion& operator=(const Quaternion& other);
	Quaternion& operator+=(const Quaternion& other);
	Quaternion& operator-=(const Quaternion& other);
	Quaternion& operator*=(float scalar);
	bool operator==(const Quaternion& other);
	bool operator!=(const Quaternion& other);
	float operator[](int i) const;
	float& operator[](int i);
	operator Vector3() const;
	operator std::string() const;

private:
	bool Compare(float x, float y, float epsilon);

private:

	Base_Class;

	Reflected_Type(Quaternion);
};

namespace Engine
{
	//Class_Inherits(Quaternion, CoreType);

	Define_Value_Type(Quaternion);
}

Quaternion operator*(float scalar, const Quaternion& quaternion);
std::ostream& operator<<(std::ostream& out, const Quaternion& quaternion);
