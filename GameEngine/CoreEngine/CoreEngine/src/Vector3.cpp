/*
Vector.Cpp

contains implimentation of Vector class functions
*/
#include "Vector3.h"

#include <iostream>
extern "C" {
#include <stdlib.h>
}
#include <sstream>
extern "C" {
#include <math.h>
}

// construct from coordinates
Vector3::Vector3(float x, float y, float z, float w)
{
	Set(x, y, z, w);
}

// construct from 2D vector
/*Vector3::Vector3(Vector2 vector)
{
	set(vector.X, vector.Y, 0, vector.W);
}*/

// set coordinates
Vector3& Vector3::Set(float x, float y, float z, float w)
{
	this->X = x;
	this->Y = y;
	this->Z = z;
	this->W = w;

	return *this;
}

// normalizes the vector
Vector3& Vector3::Normalize()
{
	float length = 1 / this->Length();
	X *= length;
	Y *= length;
	Z *= length;

	return *this;
}

float Vector3::Dot(const Vector3& other) const
{
	return *this * other;
}

// calculates the cross product between two vectors
Vector3 Vector3::Cross(const Vector3& other) const
{
	return Vector3(
		Y * other.Z - Z * other.Y,
		Z * other.X - X * other.Z,
		X * other.Y - Y * other.X
	);
}

Vector3 Vector3::Unit() const
{
	return Vector3(*this).Normalize();
}

// returns the length of the vector
float Vector3::Length() const
{
	return sqrtf(SquareLength());
}

// returns the square length of the vector
float Vector3::SquareLength() const
{
	return X * X + Y * Y + Z * Z + W * W;
}

Vector3& Vector3::Scale(const Vector3 & other)
{
  X *= other.X;
  Y *= other.Y;
  Z *= other.Z;
  return *this;
}

// negation
Vector3 Vector3::operator-() const
{
	return Vector3(-X, -Y, -Z, -W);
}

// addition
Vector3 Vector3::operator+(const Vector3& other) const
{
	return Vector3(X + other.X, Y + other.Y, Z + other.Z, W + other.W);
}

// subtraction
Vector3 Vector3::operator-(const Vector3& other) const
{
	return Vector3(X - other.X, Y - other.Y, Z - other.Z, W - other.W);
}

// scalar multiplication
Vector3 Vector3::operator*(float scalar) const
{
	return Vector3(scalar * X, scalar * Y, scalar * Z, scalar * W);
}

// dot product multiplication
float Vector3::operator*(const Vector3& other) const
{
	return X * other.X + Y * other.Y + Z * other.Z + W * other.W;
}

// assignment
Vector3& Vector3::operator=(const Vector3& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	W = other.W;

	return *this;
}

// addition assignment
Vector3& Vector3::operator+=(const Vector3& other)
{
	*this = *this + other;

	return *this;
}

// subtraction assignment
Vector3& Vector3::operator-=(const Vector3& other)
{
	*this = *this - other;

	return *this;
}

// multiplication assignment
Vector3& Vector3::operator*=(float scalar)
{
	*this = *this * scalar;

	return *this;
}

bool Vector3::Compare(float x, float y, float epsilon) const
{
	return abs(x - y) < epsilon;
}

bool Vector3::operator==(const Vector3& other) const
{
	float epsilon = 1e-5f;

	return Compare(X, other.X, epsilon) && Compare(Y, other.Y, epsilon) && Compare(Z, other.Z, epsilon);
}

bool Vector3::operator!=(const Vector3& other) const
{
	return !(*this == other);
}

float Vector3::operator[](int i) const
{
	return ((const float*)(this))[i];
}

float& Vector3::operator[](int i)
{
	return ((float*)(this))[i];
}

// rhs scalar multiplication
Vector3 operator*(float scalar, const Vector3& vector)
{
	//use other scalar multiplication function
	return vector * scalar;
}

Vector3::operator std::string() const
{
	std::stringstream out;

	out << *this;

	return out.str();
}

// stream output
std::ostream& operator<<(std::ostream& out, const Vector3& vector)
{
	if (vector.W < 0.99999 || vector.W > 1.00001)
		out << "< " << vector.X << ", " << vector.Y << ", " << vector.Z << ", " << vector.W << " >";
	else
		out << "( " << vector.X << ", " << vector.Y << ", " << vector.Z << " )";
		
	return out;
}
