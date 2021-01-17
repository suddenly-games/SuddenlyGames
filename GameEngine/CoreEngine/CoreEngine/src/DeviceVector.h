#pragma once

#include "Vector3.h"

class DeviceAxis
{
public:
	float Scale;
	float Offset;

	DeviceAxis(float scale = 0, float offset = 0) : Scale(scale), Offset(offset) {}

	void Set(float newScale, float newOffset);
	float Calculate(float base, float size) const;

	bool operator==(const DeviceAxis& other) const;
	bool operator!=(const DeviceAxis& other) const;
	DeviceAxis operator+(const DeviceAxis& axis) const;
	DeviceAxis operator-(const DeviceAxis& axis) const;
	DeviceAxis operator*(float scalar) const;
	DeviceAxis operator/(float scalar) const;
	DeviceAxis& operator+=(const DeviceAxis& axis);
	DeviceAxis& operator-=(const DeviceAxis& axis);
	DeviceAxis& operator*=(float scalar);
	DeviceAxis& operator/=(float scalar);

	Base_Class;

	Reflected_Type(DeviceAxis);
};

DeviceAxis operator*(float scalar, DeviceAxis axis);

class DeviceVector
{
public:
	DeviceAxis X;
	DeviceAxis Y;

	DeviceVector(float xScale = 0, float xOffset = 0, float yScale = 0, float yOffset = 0) : X(xScale, xOffset), Y(yScale, yOffset) {}
	DeviceVector(const DeviceAxis& x, const DeviceAxis& y) : X(x), Y(y) {}

	void Set(float xScale, float xOffset, float yScale, float yOffset);
	void Set(const DeviceAxis& xAxis, const DeviceAxis& yAxis);
	Vector3 Calculate(const Vector3& base, const Vector3& size) const;
	Vector3 Calculate(float baseX, float baseY, float sizeX, float sizeY) const;
	DeviceVector Lerp(float t, const DeviceVector& goal) const;

	bool operator==(const DeviceVector& other) const;
	bool operator!=(const DeviceVector& other) const;
	DeviceVector operator+(const DeviceVector& vector) const;
	DeviceVector operator-(const DeviceVector& vector) const;
	DeviceVector operator*(float scalar) const;
	DeviceVector operator/(float scalar) const;
	DeviceVector& operator+=(const DeviceVector& vector);
	DeviceVector& operator-=(const DeviceVector& vector);
	DeviceVector& operator*=(float scalar);
	DeviceVector& operator/=(float scalar);

	Base_Class;

	Reflected_Type(DeviceVector);
};

namespace Engine
{
	//Class_Inherits(DeviceAxis, CoreType);
	//Class_Inherits(DeviceVector, CoreType);

	Define_Value_Type(DeviceAxis);
	Define_Value_Type(DeviceVector);
}

DeviceVector operator*(float scalar, const DeviceVector& vector);





