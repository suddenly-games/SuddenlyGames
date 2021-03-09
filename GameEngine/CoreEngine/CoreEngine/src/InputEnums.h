#pragma once

struct InputStateEnum
{
	enum InputState
	{
		Began,
		Changed,
		Ended,

		Idle,
		Active,

		None
	};
};

struct InputTypeEnum
{
	enum InputType
	{
		Button,
		Point,

		Custom,

		None
	};
};

struct InputCodeEnum
{
	enum InputCode
	{
		MouseLeft,
		MouseRight,
		MouseMiddle,
		MouseWheel,
		MousePosition,

		NumpadZero,
		NumpadOne,
		NumpadTwo,
		NumpadThree,
		NumpadFour,
		NumpadFive,
		NumpadSix,
		NumpadSeven,
		NumpadEight,
		NumpadNine,

		NumpadSlash,
		NumpadAstrisk,
		NumpadHyphen,
		NumpadPlus,
		NumpadEnter,
		NumpadPeriod,

		BracketOpen,
		BracketClose,
		Backslash,
		Semicolon,
		Apostrophe,
		Comma,
		Period,
		Slash,
		GraveAccent,
		Hyphen,
		Equals,

		Space = ' ',

		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,

		Zero = '0',
		One = '1',
		Two = '2',
		Three = '3',
		Four = '4',
		Five = '5',
		Six = '6',
		Seven = '7',
		Eight = '8',
		Nine = '9',

		Escape,
		Tab,
		CapsLock,
		LeftShift,
		RightShift,
		LeftControl,
		RightControl,
		LeftAlt,
		RightAlt,
		Enter,
		Backspace,
		Insert,
		DeleteButton,
		Home,
		End,
		PageUp,
		PageDown,
		NumLock,

		UpArrow,
		DownArrow,
		LeftArrow,
		RightArrow,

		A = 'a',
		B = 'b',
		C = 'c',
		D = 'd',
		E = 'e',
		F = 'f',
		G = 'g',
		H = 'h',
		I = 'i',
		J = 'j',
		K = 'k',
		L = 'l',
		M = 'm',
		N = 'n',
		O = 'o',
		P = 'p',
		Q = 'q',
		R = 'r',
		S = 's',
		T = 't',
		U = 'u',
		V = 'v',
		W = 'w',
		X = 'x',
		Y = 'y',
		Z = 'z',

		Custom,

		None,
		Codes
	};
};

struct BoundDeviceEnum
{
	enum BoundDevice
	{
		Any,
		Mouse1,
		Input1,

		Count
	};
};

namespace Enum
{
	typedef InputStateEnum::InputState InputState;
	typedef InputTypeEnum::InputType InputType;
	typedef InputCodeEnum::InputCode InputCode;
	typedef BoundDeviceEnum::BoundDevice BoundDevice;
}

extern const Enum::InputType inputTypes[Enum::InputCode::Codes];
extern const char* inputNames[Enum::InputCode::Codes];
extern const Enum::BoundDevice inputDevices[Enum::InputCode::Codes];
