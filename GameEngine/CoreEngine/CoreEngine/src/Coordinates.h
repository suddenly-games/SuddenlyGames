#pragma once

#include "Vector3.h"

namespace Engine
{
	struct Coordinates
	{
		int X = 0;
		int Y = 0;
		int Z = 0;

		Coordinates(int x = 0, int y = 0, int z = 0) : X(x), Y(y), Z(z) {}
		Coordinates(const Vector3& vector) : X(int(std::floorf(vector.X))), Y(int(std::floorf(vector.Y))), Z(int(std::floorf(vector.Z))) {}

		operator Vector3() const { return Vector3(float(X), float(Y), float(Z)); }
	};
}