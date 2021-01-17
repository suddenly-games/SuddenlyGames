#include "InputEvent.h"

bool InputObject::GetState() const
{
	return IsDown;
}

bool InputObject::GetStateChanged() const
{
	return StateChanged;
}

const Vector3& InputObject::GetPosition() const
{
	return Position;
}

const Vector3& InputObject::GetDelta() const
{
	return Delta;
}

Enum::InputType InputObject::GetType() const
{
	return Type;
}

Enum::InputCode InputObject::GetCode() const
{
	return Code;
}

std::string InputObject::GetName() const
{
	return Name;
}

bool InputHandler::GetState(Enum::InputCode code) const
{
	return Inputs[code].IsDown;
}

bool InputHandler::GetStateChanged(Enum::InputCode code) const
{
	return Inputs[code].StateChanged;
}

const Vector3& InputHandler::GetPosition(Enum::InputCode code) const
{
	return Inputs[code].Position;
}

const Vector3& InputHandler::GetDelta(Enum::InputCode code) const
{
	return Inputs[code].Delta;
}

Enum::InputType InputHandler::GetType(Enum::InputCode code) const
{
	return Inputs[code].Type;
}

const char* InputHandler::GetName(Enum::InputCode code) const
{
	return Inputs[code].Name;
}

InputObject& InputHandler::GetInput(Enum::InputCode code)
{
	return Inputs[code];
}

const InputObject& InputHandler::GetInput(Enum::InputCode code) const
{
	return Inputs[code];
}

void InputHandler::EventHandler::ProcessEvent(SDL_Event& event)
{
	Target->ProcessEvent(event);
}

void InputHandler::EventHandler::FlushQueue()
{
	Target->FlushQueue();
}

void InputHandler::EventHandler::ResetDeltas()
{
	if (Target != nullptr)
		Target->ResetDeltas();
}

InputHandler::InputHandler(EventHandler& eventHandler)
{
	eventHandler.Target = this;

	for (int i = 0; i < Enum::InputCode::Codes; ++i)
	{
		Inputs[i].Type = inputTypes[i];
		Inputs[i].Code = Enum::InputCode(i);
		Inputs[i].Name = inputNames[i];
	}
}

void InputHandler::ProcessKey(int keyCode, bool state)
{
	if ((keyCode >= '0' && keyCode <= '9') || (keyCode >= 'a' && keyCode <= 'z'))
		SetInputState(Inputs[keyCode], state);
	else if (keyCode >= SDLK_KP_1 && keyCode <= SDLK_KP_9)
		SetInputState(Inputs[Enum::InputCode::NumpadOne + (keyCode - SDLK_KP_1)], state);
	else if (keyCode >= SDLK_F1 && keyCode <= SDLK_F12)
		SetInputState(Inputs[Enum::InputCode::F1 + (keyCode - SDLK_F1)], state);
	else
	{
		switch (keyCode)
		{
		case SDLK_KP_0: SetInputState(Inputs[Enum::InputCode::NumpadZero], state); break;
		case SDLK_KP_DIVIDE: SetInputState(Inputs[Enum::InputCode::NumpadSlash], state); break;
		case SDLK_KP_MULTIPLY: SetInputState(Inputs[Enum::InputCode::NumpadAstrisk], state); break;
		case SDLK_KP_MINUS: SetInputState(Inputs[Enum::InputCode::NumpadHyphen], state); break;
		case SDLK_KP_PLUS: SetInputState(Inputs[Enum::InputCode::NumpadPlus], state); break;
		case SDLK_KP_ENTER: SetInputState(Inputs[Enum::InputCode::NumpadEnter], state); break;
		case SDLK_KP_PERIOD: SetInputState(Inputs[Enum::InputCode::NumpadPeriod], state); break;
		case SDLK_LEFTBRACKET: SetInputState(Inputs[Enum::InputCode::BracketOpen], state); break;
		case SDLK_RIGHTBRACKET: SetInputState(Inputs[Enum::InputCode::BracketClose], state); break;
		case SDLK_BACKSLASH: SetInputState(Inputs[Enum::InputCode::Backslash], state); break;
		case SDLK_QUOTE: SetInputState(Inputs[Enum::InputCode::Apostrophe], state); break;
		case SDLK_COMMA: SetInputState(Inputs[Enum::InputCode::Comma], state); break;
		case SDLK_PERIOD: SetInputState(Inputs[Enum::InputCode::Period], state); break;
		case SDLK_SLASH: SetInputState(Inputs[Enum::InputCode::Slash], state); break;
		case SDLK_BACKQUOTE: SetInputState(Inputs[Enum::InputCode::GraveAccent], state); break;
		case SDLK_MINUS: SetInputState(Inputs[Enum::InputCode::Hyphen], state); break;
		case SDLK_EQUALS: SetInputState(Inputs[Enum::InputCode::Equals], state); break;
		case SDLK_SPACE: SetInputState(Inputs[Enum::InputCode::Space], state); break;
		case SDLK_ESCAPE: SetInputState(Inputs[Enum::InputCode::Escape], state); break;
		case SDLK_TAB: SetInputState(Inputs[Enum::InputCode::Tab], state); break;
		case SDLK_CAPSLOCK: SetInputState(Inputs[Enum::InputCode::CapsLock], state); break;
		case SDLK_LSHIFT: SetInputState(Inputs[Enum::InputCode::LeftShift], state); break;
		case SDLK_RSHIFT: SetInputState(Inputs[Enum::InputCode::RightShift], state); break;
		case SDLK_LCTRL: SetInputState(Inputs[Enum::InputCode::LeftControl], state); break;
		case SDLK_RCTRL: SetInputState(Inputs[Enum::InputCode::RightControl], state); break;
		case SDLK_LALT: SetInputState(Inputs[Enum::InputCode::LeftAlt], state); break;
		case SDLK_RALT: SetInputState(Inputs[Enum::InputCode::RightAlt], state); break;
		case SDLK_RETURN: SetInputState(Inputs[Enum::InputCode::Enter], state); break;
		case SDLK_BACKSPACE: SetInputState(Inputs[Enum::InputCode::Backspace], state); break;
		case SDLK_INSERT: SetInputState(Inputs[Enum::InputCode::Insert], state); break;
		case SDLK_DELETE: SetInputState(Inputs[Enum::InputCode::DeleteButton], state); break;
		case SDLK_HOME: SetInputState(Inputs[Enum::InputCode::Home], state); break;
		case SDLK_END: SetInputState(Inputs[Enum::InputCode::End], state); break;
		case SDLK_PAGEUP: SetInputState(Inputs[Enum::InputCode::PageUp], state); break;
		case SDLK_PAGEDOWN: SetInputState(Inputs[Enum::InputCode::PageDown], state); break;
		case SDLK_NUMLOCKCLEAR: SetInputState(Inputs[Enum::InputCode::NumLock], state); break;
		case SDLK_UP: SetInputState(Inputs[Enum::InputCode::UpArrow], state); break;
		case SDLK_DOWN: SetInputState(Inputs[Enum::InputCode::DownArrow], state); break;
		case SDLK_LEFT: SetInputState(Inputs[Enum::InputCode::LeftArrow], state); break;
		case SDLK_RIGHT: SetInputState(Inputs[Enum::InputCode::RightArrow], state); break;
		}
	}
}

void InputHandler::ProcessMouse(int mouseCode, bool state)
{
	switch (mouseCode)
	{
	case SDL_BUTTON_LEFT:
		SetInputState(Inputs[Enum::InputCode::MouseLeft], state);

		break;
	case SDL_BUTTON_MIDDLE:
		SetInputState(Inputs[Enum::InputCode::MouseMiddle], state);

		break;
	case SDL_BUTTON_RIGHT:
		SetInputState(Inputs[Enum::InputCode::MouseRight], state);

		break;
	}
}

void InputHandler::ProcessEvent(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
		ProcessKey(event.key.keysym.sym, true);

		break;
	case SDL_KEYUP:
		ProcessKey(event.key.keysym.sym, false);

		break;
	case SDL_MOUSEBUTTONDOWN:
		ProcessMouse(event.button.button, true);

		break;
	case SDL_MOUSEBUTTONUP:
		ProcessMouse(event.button.button, false);

		break;
	case SDL_MOUSEWHEEL:
		Inputs[Enum::InputCode::MouseWheel].Delta.Set(float(event.wheel.x), float(event.wheel.y));
		Inputs[Enum::InputCode::MouseWheel].Delta *= event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? -1.0f : 1.0f;
		EventQueue.push_back(InputEvent{ Enum::InputCode::MouseWheel, Enum::InputState::Changed });

		break;
	case SDL_MOUSEMOTION:
		Inputs[Enum::InputCode::MousePosition].Position.Set(float(event.motion.x), float(event.motion.y));
		Inputs[Enum::InputCode::MousePosition].Delta.Set(float(event.motion.xrel), float(event.motion.yrel));
		EventQueue.push_back(InputEvent{ Enum::InputCode::MousePosition, Enum::InputState::Changed });

		break;
	}
}

void InputHandler::SetInputState(InputObject& input, bool state)
{
	input.StateChanged = state != input.IsDown;
	input.IsDown = state;

	if (input.StateChanged)
	{
		if (input.IsDown)
			EventQueue.push_back(InputEvent{ input.Code, Enum::InputState::Began });
		else
			EventQueue.push_back(InputEvent{ input.Code, Enum::InputState::Ended });

		EventQueue.push_back(InputEvent{ input.Code, Enum::InputState::Changed });
	}
}

void InputHandler::FlushQueue()
{
	for (int i = 0; i < int(EventQueue.size()); ++i)
	{
		switch (EventQueue[i].State)
		{
		case Enum::InputState::Began:
			Inputs[EventQueue[i].Code].Began.Fire(&Inputs[EventQueue[i].Code]);

			break;
		case Enum::InputState::Changed:
			Inputs[EventQueue[i].Code].Changed.Fire(&Inputs[EventQueue[i].Code]);

			break;
		case Enum::InputState::Ended:
			Inputs[EventQueue[i].Code].Ended.Fire(&Inputs[EventQueue[i].Code]);
		}
	}

	EventQueue.clear();
}

void InputHandler::ResetDeltas()
{
	for (int i = 0; i < Enum::InputCode::Codes; ++i)
		Inputs[i].Delta.Set();
}