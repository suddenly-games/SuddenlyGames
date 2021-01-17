#pragma once

#include "Vector3.h"
extern "C" {
#include <SDL.h>
}

struct Mouse
{
	bool LeftButton = false;
	bool MiddleButton = false;
	bool RightButton = false;

	Vector3 Position;
	Vector3 LastPosition;
	Vector3 Delta;
};

struct Keyboard
{
	const Uint8* Keys = nullptr;

	bool IsKeyDown(char key) const;
};
