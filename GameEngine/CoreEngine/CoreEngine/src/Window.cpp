#include "Window.h"

extern "C" {
#include <Windows.h>
}

#define GL3_PROTOTYPES 1
extern "C" {
#include <glew.h>
}
#include "ShaderProgram.h"
#include "FrameBuffer.h"
#include "Graphics.h"

Window::Window() : Input(new InputHandler(EventHandler))
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw "failed to initialize SDL";

	SDL_DisplayMode display;
	SDL_GetCurrentDisplayMode(0, &display);
	Resolution.Set(float(display.w), float(display.h));
	GraphicsEngine::FrameBuffer::WindowSize = Dimensions((unsigned int)(Resolution.X), (unsigned int)(Resolution.Y));
	RefreshRate = display.refresh_rate;

	WindowHandle = SDL_CreateWindow("OpenGL renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, display.w, display.h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

	if (WindowHandle == nullptr)
		throw "failed to create window";

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // disable deprecated code
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); // set to opengl 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // enable double buffering

	Context = SDL_GL_CreateContext(WindowHandle); // attach opengl to the SDL window

	glewExperimental = GL_TRUE; // use opengl 3.0+ stuff

	SDL_GL_SetSwapInterval(1);
	glewInit();

	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST); CheckGLErrors();
	glEnable(GL_CULL_FACE); CheckGLErrors();
	glEnable(GL_BLEND); CheckGLErrors();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); CheckGLErrors();
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

	int x, y;
	SDL_GetMouseState(&x, &y);

	MouseInput.Position.Set(float(x), float(y));

	LastTime = SDL_GetTicks();

	DEVMODEA settings;

	if (EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &settings))
		RefreshDelta = 1 / float(settings.dmDisplayFrequency + 5);
}

Window::~Window()
{
	SDL_GL_DeleteContext(Context);
	SDL_DestroyWindow(WindowHandle);
	SDL_Quit();
}

float Window::Update()
{
	//SDL_Timer(1);

	MouseInput.LastPosition = MouseInput.Position;

	glClearColor(0, 0, 0, 1); CheckGLErrors();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); CheckGLErrors();

	EventHandler.ResetDeltas();

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		EventHandler.ProcessEvent(event);

		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				Closed = true;
				break;

			default:
				break;
			}
		}
		else if (event.type == SDL_QUIT)
			Closed = true;
		else if (event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEBUTTONDOWN)
		{
			bool state = event.type == SDL_MOUSEBUTTONDOWN;

			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				MouseInput.LeftButton = state;

				break;
			case SDL_BUTTON_MIDDLE:
				MouseInput.MiddleButton = state;

				break;
			case SDL_BUTTON_RIGHT:
				MouseInput.RightButton = state;

				break;
			default:
				break;
			}
		}
	}

	int x, y;
	SDL_GetMouseState(&x, &y);

	MouseInput.Position.Set(float(x), float(y), 0, 1);
	MouseInput.Delta = MouseInput.Position - MouseInput.LastPosition;
	KeyboardInput.Keys = SDL_GetKeyboardState(0);

	GraphicsEngine::FrameBuffer::WindowSize = Dimensions((unsigned int)(Resolution.X), (unsigned int)(Resolution.Y));

	EventHandler.FlushQueue();

	return Delta;
}

void Window::Swap()
{
	LastDraw = Time;
	SDL_GL_SwapWindow(WindowHandle);

	Uint64 currentTime = SDL_GetTicks();

	Delta = float(currentTime - LastTime) / 1000;
	LastTime = currentTime;

	Time += Delta;

}

const Mouse& Window::GetMouse() const
{
	return MouseInput;
}

const Keyboard& Window::GetKeyboard() const
{
	return KeyboardInput;
}

bool Window::IsClosed() const
{
	return Closed;
}

float Window::GetDeltaTime()const
{
	return Delta;
}

float Window::GetTime() const
{
	return Time;
}

void Window::SetMousePosition(const Vector3& position)
{
	SetMousePosition(int(position.X), int(position.Y));
}

void Window::SetMousePosition(int x, int y)
{
	SDL_WarpMouseGlobal(x, y);

	MouseInput.Position.Set(float(x), float(y));
}

bool Window::NextFrameReady() const
{
	return Time > LastDraw + RefreshDelta;
}

void Window::Wait(int milliseconds)
{
	SDL_Delay(milliseconds);

	Uint64 currentTime = SDL_GetTicks();

	Delta = float(currentTime - LastTime) / 1000;
	LastTime = currentTime;

	Time += Delta;
}

float Window::GetRefreshDelta() const
{
	return RefreshDelta;
}
