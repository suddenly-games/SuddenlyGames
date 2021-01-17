#include "Input.h"

bool Keyboard::IsKeyDown(char key) const
{
	int code = -1;

	if (key >= 'a' && key <= 'z')
		code = SDL_SCANCODE_A + key - 'a';
	else if (key >= 'A' && key <= 'Z')
		code = SDL_SCANCODE_A + key - 'A';
	else if (key >= '1' && key <= '9')
		code = SDL_SCANCODE_1 + key - '1';
	else if (key == '0')
		code = SDL_SCANCODE_0;

	if (code != -1)
		return Keys[code] != 0;

	return false;
}
