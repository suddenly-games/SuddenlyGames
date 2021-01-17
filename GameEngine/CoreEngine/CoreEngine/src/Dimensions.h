#pragma once

#include <string>

#include "Vector3.h"
#include "ObjectReflection.h"

struct Dimensions
{
	unsigned int Width;
	unsigned int Height;

	Dimensions(unsigned int width = 0, unsigned int height = 0) : Width(width), Height(height) {}
	Dimensions(const Vector3& size) : Width(unsigned int(size.X)), Height(unsigned int(size.Y)) {}

	operator std::string();
	bool operator==(const Dimensions& other) { return Width == other.Width && Height == other.Height; }
	bool operator!=(const Dimensions& other) { return Width != other.Width || Height != other.Height; }

	Base_Class;

	Reflected_Type(Dimensions);
};

namespace Engine
{
	Define_Value_Type(Dimensions);
}