#pragma once

extern "C" {
#include <SDL.h>
}

#include "Matrix3.h"
#include "Input.h"
#include "InputEvent.h"

class Window
{
public:
	int RefreshRate;
	Vector3 Resolution;
	InputHandler* Input;

	Window();
	~Window();

	float Update();
	void Swap();
	const Mouse& GetMouse() const;
	const Keyboard& GetKeyboard() const;
	bool IsClosed() const;
	float GetDeltaTime() const;
	float GetTime() const;
	void SetMousePosition(const Vector3& position);
	void SetMousePosition(int x, int y);
	bool NextFrameReady() const;
	void Wait(int milliseconds);
	float GetRefreshDelta() const;

private:
	SDL_Window* WindowHandle = nullptr;
	SDL_GLContext Context;
	InputHandler::EventHandler EventHandler;
	Mouse MouseInput;
	Keyboard KeyboardInput;
	bool Closed = false;
	Uint64 LastTime;
	float Delta = 0;
	float Time = 0;
	float LastDraw = 0;
	float RefreshDelta = 0;
};
