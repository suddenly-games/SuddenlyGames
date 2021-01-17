/*
Vector3.H

contains declaration of Vector3 class
*/

#pragma once

#include <iostream>
#include <string>

#include "ObjectReflection.h"

class Vector3
{
public:
	float X, Y, Z, W;

	Vector3(float x = 0, float y = 0, float z = 0, float w = 0);
	//Vector3(Vector2 vector);

	Vector3& Set(float x = 0, float y = 0, float z = 0, float w = 0);
	Vector3& Normalize();
	float Dot(const Vector3& other) const;
	Vector3 Cross(const Vector3& other) const;
	Vector3 Unit() const;
	float Length() const;
	float SquareLength() const;
  Vector3& Scale(const Vector3& other);

	Vector3 Negate() const { return -*this; }

	Vector3 operator-() const;
	Vector3 operator+(const Vector3& other) const;
	Vector3 operator-(const Vector3& other) const;
	Vector3 operator*(float scalar) const;
	float operator*(const Vector3& other) const;
	Vector3& operator=(const Vector3& other);
	Vector3& operator+=(const Vector3& other);
	Vector3& operator-=(const Vector3& other);
	Vector3& operator*=(float scalar);
	bool operator==(const Vector3& other) const;
	bool operator!=(const Vector3& other) const;
	float operator[](int i) const;
	float& operator[](int i);

	operator std::string() const;

private:
	bool Compare(float x, float y, float epsilon)  const;

	Base_Class;

	Reflected_Type(Vector3);
};

namespace Engine
{
	//Class_Inherits(Vector3, CoreType);

	Define_Value_Type(Vector3);
}

Vector3 operator*(float scalar, const Vector3& vector);
std::ostream& operator<<(std::ostream& out, const Vector3& vector);
