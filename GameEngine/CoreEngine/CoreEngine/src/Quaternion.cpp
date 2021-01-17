#include "Quaternion.h"

Quaternion::Quaternion(const Matrix3& matrix)
{
	float traceW = matrix[0][0] + matrix[1][1] + matrix[2][2];
	float traceX = matrix[0][0] - matrix[1][1] - matrix[2][2];
	float traceY = -matrix[0][0] + matrix[1][1] - matrix[2][2];
	float traceZ = -matrix[0][0] - matrix[1][1] + matrix[2][2];

	float axis1 = (matrix[2][1] + matrix[1][2]);
	float axis2 = (matrix[0][2] + matrix[2][0]);
	float axis3 = (matrix[1][0] + matrix[0][1]);

	float axis1N = (matrix[2][1] - matrix[1][2]);
	float axis2N = (matrix[0][2] - matrix[2][0]);
	float axis3N = (matrix[1][0] - matrix[0][1]);

	if (traceW > traceX && traceW > traceY && traceW > traceZ)
	{
		W = 0.5f * sqrtf(1 + traceW);
		X = axis1N;
		Y = axis2N;
		Z = axis3N;

		X /= 4 * W;
		Y /= 4 * W;
		Z /= 4 * W;
	}
	else if (traceX > traceY && traceX > traceZ)
	{
		W = axis1N;
		X = 0.5f * sqrtf(1 + traceX);
		Y = axis3;
		Z = axis2;

		W /= 4 * X;
		Y /= 4 * X;
		Z /= 4 * X;
	}
	else if (traceY > traceZ)
	{
		W = axis2N;
		X = axis3;
		Y = 0.5f * sqrtf(1 + traceY);
		Z = axis1;

		W /= 4 * Y;
		X /= 4 * Y;
		Z /= 4 * Y;
	}
	else
	{
		W = axis3N;
		X = axis2;
		Y = axis1;
		Z = 0.5f * sqrtf(1 + traceZ);

		W /= 4 * Z;
		X /= 4 * Z;
		Y /= 4 * Z;
	}

	Normalize();
}

Quaternion& Quaternion::Invert()
{
	X *= -1;
	Y *= -1;
	Z *= -1;

	return *this;
}

Quaternion Quaternion::Slerp(const Quaternion& destination, float t) const
{

	float halfCos = Dot(destination);
	float scalar = 1;
	
	if (halfCos >= 1 || halfCos <= -1)
		return *this;
	else if (halfCos < 0)
		scalar = -1;

	halfCos *= scalar;
	
	float halfTheta = acosf(halfCos);
	float halfSin = sqrtf(1 - halfCos * halfCos);
	
	if (halfSin < 1e-3f && halfSin > -1e-3f)
		return 0.5f * (*this + scalar * destination);
	
	float ratioA = sinf((1 - t) * halfTheta) / halfSin;
	float ratioB = scalar * sinf(t * halfTheta) / halfSin;
	
	return (ratioA * *this + ratioB * destination).Normalize();
}

Quaternion Quaternion::operator*(const Quaternion& rhs) const
{
	return Quaternion(
		W * rhs.X + X * rhs.W + Y * rhs.Z - Z * rhs.Y,
		W * rhs.Y - X * rhs.Z + Y * rhs.W + Z * rhs.X,
		W * rhs.Z + X * rhs.Y - Y * rhs.X + Z * rhs.W,
		W * rhs.W - X * rhs.X - Y * rhs.Y - Z * rhs.Z
	);
}

//Vector3 Quaternion::operator*(const Vector3& rhs) const
//{
//	float a = 2 * (rhs * (Vector3)(*this));
//	float b = w * w - Quaternion(x, y, z).SquareLength();
//	float c = 2 * w;
//
//	Quaternion product = cross(rhs);
//
//	return a * Vector3(x, y, z) + b * rhs + c * Vector3(product.X, product.Y, product.Z);
//}

Matrix3 Quaternion::Matrix() const
{
	float xx = X * X;
	float yy = Y * Y;
	float zz = Z * Z;
	float xz = X * Z;
	float xy = X * Y;
	float yz = Y * Z;
	float wx = W * X;
	float wy = W * Y;
	float wz = W * Z;

	return Matrix3(
		Vector3(0, 0, 0, 1),
		Vector3(1 - 2 * (yy + zz), 2 * (xy + wz), 2 * (xz - wy)).Normalize(),
		Vector3(2 * (xy - wz), 1 - 2 * (xx + zz), 2 * (yz + wx)).Normalize(),
		Vector3(2 * (xz + wy), 2 * (yz - wx), 1 - 2 * (xx + yy)).Normalize()
	);
}

// normalizes the vector
Quaternion& Quaternion::Normalize()
{
	float length = 1 / this->Length();
	W *= length;
	X *= length;
	Y *= length;
	Z *= length;

	return *this;
}

// calculates the cross product between two vectors
Quaternion Quaternion::Cross(const Quaternion& other) const
{
	return Vector3(
		Y * other.Z - Z * other.Y,
		Z * other.X - X * other.Z,
		X * other.Y - Y * other.X
	);
}

Quaternion Quaternion::Unit() const
{
	return Quaternion(*this).Normalize();
}

// returns the length of the vector
float Quaternion::Length() const
{
	return sqrtf(SquareLength());
}

// returns the square length of the vector
float Quaternion::SquareLength() const
{
	return Dot(*this);
}

float Quaternion::Dot(const Quaternion& other) const
{
	return X * other.X + Y * other.Y + Z * other.Z + W * other.W;
}

// negation
Quaternion Quaternion::operator-() const
{
	return Quaternion(-W, -X, -Y, -Z);
}

// addition
Quaternion Quaternion::operator+(const Quaternion& other) const
{
	return Quaternion(W + other.W, X + other.X, Y + other.Y, Z + other.Z);
}

// subtraction
Quaternion Quaternion::operator-(const Quaternion& other) const
{
	return Quaternion(W - other.W, X - other.X, Y - other.Y, Z - other.Z);
}

// scalar multiplication
Quaternion Quaternion::operator*(float scalar) const
{
	return Quaternion(scalar * W, scalar * X, scalar * Y, scalar * Z);
}

// assignment
Quaternion& Quaternion::operator=(const Quaternion& other)
{
	W = other.X;
	X = other.Y;
	Y = other.Z;
	Z = other.W;

	return *this;
}

// addition assignment
Quaternion& Quaternion::operator+=(const Quaternion& other)
{
	*this = *this + other;

	return *this;
}

// subtraction assignment
Quaternion& Quaternion::operator-=(const Quaternion& other)
{
	*this = *this - other;

	return *this;
}

// multiplication assignment
Quaternion& Quaternion::operator*=(float scalar)
{
	*this = *this * scalar;

	return *this;
}

bool Quaternion::Compare(float x, float y, float epsilon)
{
	float a = x - y;

	return a < epsilon && a > -epsilon;
}

bool Quaternion::operator==(const Quaternion& other)
{
	float epsilon = 1e-5f;

	return Compare(X, other.X, epsilon) && Compare(Y, other.Y, epsilon) && Compare(Z, other.Z, epsilon) && Compare(W, other.W, epsilon);
}

bool Quaternion::operator!=(const Quaternion& other)
{
	return !(*this == other);
}

float Quaternion::operator[](int i) const
{
	return ((const float*)(this))[i];
}

float& Quaternion::operator[](int i)
{
	return ((float*)(this))[i];
}

Quaternion::operator Vector3() const
{
	return Vector3(X, Y, Z, W);
}

Quaternion operator*(float scalar, const Quaternion& quaternion)
{
	return quaternion * scalar;
}

Quaternion::operator std::string() const
{
	std::stringstream out;

	out << *this;

	return out.str();
}

std::ostream& operator<<(std::ostream& out, const Quaternion& quaternion)
{
	return out << "( " << quaternion.W << " + " << quaternion.X << "i + " << quaternion.Y << "j + " << quaternion.Z << "k )";
}
