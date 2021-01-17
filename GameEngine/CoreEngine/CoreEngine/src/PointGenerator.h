#pragma once

#include "Vector3.h"
#include "Constants.h"

class VectorGenerator
{
public:
	virtual Vector3 Generate() {
		return Vector3();
	}

	static float randf()
	{
		return float(rand()) / RAND_MAX;
	}

	Base_Class;

	Reflected_Type(VectorGenerator);
};

struct NumberRange
{
	float Min;
	float Max;

	float rand()
	{
		return Min + VectorGenerator::randf() * (Max - Min);
	}

	void Set(float min, float max)
	{
		Min = min;
		Max = max;
	}

	NumberRange() : Min(0), Max(1) {}
	NumberRange(float min) : Min(min), Max(min) {}
	NumberRange(float min, float max) : Min(min), Max(max) {}
	NumberRange(const NumberRange& range) : Min(range.Min), Max(range.Max) {}

	NumberRange& operator=(float value)
	{
		Min = value;
		Max = value;
	}

	Base_Class;

	Reflected_Type(NumberRange);
};

namespace Engine
{
	//Class_Inherits(VectorGenerator, CoreType);
	//Class_Inherits(NumberRange, CoreType);

	Define_Value_Type(VectorGenerator);
	Define_Value_Type(NumberRange);
}

class PointGenerator : public VectorGenerator
{
public:
	Vector3 Position;

	PointGenerator(const Vector3& position = Vector3()) : Position(position) {}

	Vector3 Generate()
	{
		return Position;
	}

	Inherits_Type(VectorGenerator);

	Reflected_Type(PointGenerator);
};

class UnitVectorGenerator : public VectorGenerator
{
public:
	Vector3 Generate()
	{
		float theta = 2 * PI * randf();
		float phi = acos(1 - 2 * randf());
		float sinPhi = sin(phi);

		return Vector3(sinPhi * cos(theta), sinPhi * sin(theta), cos(phi));
	}

	Inherits_Type(VectorGenerator);

	Reflected_Type(UnitVectorGenerator);
};

class ConeUnitVectorGenerator : public VectorGenerator
{
public:
	Vector3 Normal = Vector3(0, 1, 0);
	NumberRange Angle;

	Vector3 Generate()
	{
		float ratio = (1 - Normal.Y) * (Normal.X * Normal.X + Normal.Z * Normal.Z);
		float shared = -ratio * Normal.X * Normal.Z;
		Vector3 vector1(shared, Normal.Z, ratio * Normal.X * Normal.X + Normal.Y);
		Vector3 vector2(ratio * Normal.Z * Normal.Z + Normal.Y, Normal.Z, shared);

		float theta = NumberRange(0, 2 * PI).rand();

		Vector3 direction = cosf(theta) * vector1 + sinf(theta) * vector2;

		float rangeStart = Angle.Min / Angle.Max;

		float theta2 = Angle.Min + (Angle.Max - Angle.Min) * acosf(rangeStart + VectorGenerator::randf() * (1 - rangeStart)) * 2 / PI;

		return (cosf(theta2) * Normal + sinf(theta2) * direction);
	}

	Inherits_Type(VectorGenerator);

	Reflected_Type(ConeUnitVectorGenerator);
};

namespace Engine
{
	//Class_Inherits(PointGenerator, VectorGenerator);
	//Class_Inherits(UnitVectorGenerator, VectorGenerator);
	//Class_Inherits(ConeUnitVectorGenerator, VectorGenerator);

	Define_Value_Type(PointGenerator);
	Define_Value_Type(UnitVectorGenerator);
	Define_Value_Type(ConeUnitVectorGenerator);
}
