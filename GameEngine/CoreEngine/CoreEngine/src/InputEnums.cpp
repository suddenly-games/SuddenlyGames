#include "InputEnums.h"

const Enum::InputType inputTypes[Enum::InputCode::Codes] = {
	Enum::InputType::Button, // mouseLeft,
	Enum::InputType::Button, // mouseRight,
	Enum::InputType::Button, // mouseMiddle,
	Enum::InputType::Point, // mouseWheel,
	Enum::InputType::Point, // mousePosition,

	Enum::InputType::Button, // numpadZero,
	Enum::InputType::Button, // numpadOne,
	Enum::InputType::Button, // numpadTwo,
	Enum::InputType::Button, // numpadThree,
	Enum::InputType::Button, // numpadFour,
	Enum::InputType::Button, // numpadFive,
	Enum::InputType::Button, // numpadSix,
	Enum::InputType::Button, // numpadSeven,
	Enum::InputType::Button, // numpadEight,
	Enum::InputType::Button, // numpadNine,

	Enum::InputType::Button, // numpadSlash,
	Enum::InputType::Button, // numpadAstrisk,
	Enum::InputType::Button, // numpadHyphen,
	Enum::InputType::Button, // numpadPlus,
	Enum::InputType::Button, // numpadEnter,
	Enum::InputType::Button, // numpadPeriod,

	Enum::InputType::Button, // bracketOpen,
	Enum::InputType::Button, // bracketClose,
	Enum::InputType::Button, // backslash,
	Enum::InputType::Button, // semicolon,
	Enum::InputType::Button, // apostrophe,
	Enum::InputType::Button, // comma,
	Enum::InputType::Button, // period,
	Enum::InputType::Button, // slash,
	Enum::InputType::Button, // graveAccent,
	Enum::InputType::Button, // hyphen,
	Enum::InputType::Button, // equals,

	Enum::InputType::Button, // space = ' ',

	Enum::InputType::Button, // f1,
	Enum::InputType::Button, // f2,
	Enum::InputType::Button, // f3,
	Enum::InputType::Button, // f4,
	Enum::InputType::Button, // f5,
	Enum::InputType::Button, // f6,
	Enum::InputType::Button, // f7,
	Enum::InputType::Button, // f8,
	Enum::InputType::Button, // f9,
	Enum::InputType::Button, // f10,
	Enum::InputType::Button, // f11,
	Enum::InputType::Button, // f12,


	Enum::InputType::Button, // [45]
	Enum::InputType::Button, // [46]
	Enum::InputType::Button, // [47]

	Enum::InputType::Button, // zero = '0',
	Enum::InputType::Button, // one = '1',
	Enum::InputType::Button, // two = '2',
	Enum::InputType::Button, // three = '3',
	Enum::InputType::Button, // four = '4',
	Enum::InputType::Button, // five = '5',
	Enum::InputType::Button, // six = '6',
	Enum::InputType::Button, // seven = '7',
	Enum::InputType::Button, // eight = '8',
	Enum::InputType::Button, // nine = '9',

	Enum::InputType::Button, // escape,
	Enum::InputType::Button, // tab,
	Enum::InputType::Button, // capsLock,
	Enum::InputType::Button, // leftShift,
	Enum::InputType::Button, // rightShift,
	Enum::InputType::Button, // leftControl,
	Enum::InputType::Button, // rightControl,
	Enum::InputType::Button, // leftAlt,
	Enum::InputType::Button, // rightAlt,
	Enum::InputType::Button, // enter,
	Enum::InputType::Button, // backspace,
	Enum::InputType::Button, // insert,
	Enum::InputType::Button, // deleteButton,
	Enum::InputType::Button, // home,
	Enum::InputType::Button, // end,
	Enum::InputType::Button, // pageUp,
	Enum::InputType::Button, // pageDown,
	Enum::InputType::Button, // numLock,

	Enum::InputType::Button, // upArrow,
	Enum::InputType::Button, // downArrow,
	Enum::InputType::Button, // leftArrow,
	Enum::InputType::Button, // rightArrow,

	Enum::InputType::Button, // [80]
	Enum::InputType::Button, // [81]
	Enum::InputType::Button, // [82]
	Enum::InputType::Button, // [83]
	Enum::InputType::Button, // [84]
	Enum::InputType::Button, // [85]
	Enum::InputType::Button, // [86]
	Enum::InputType::Button, // [87]
	Enum::InputType::Button, // [88]
	Enum::InputType::Button, // [89]
	Enum::InputType::Button, // [90]
	Enum::InputType::Button, // [91]
	Enum::InputType::Button, // [92]
	Enum::InputType::Button, // [93]
	Enum::InputType::Button, // [94]
	Enum::InputType::Button, // [95]
	Enum::InputType::Button, // [96]

	Enum::InputType::Button, // a = 'a',
	Enum::InputType::Button, // b = 'b',
	Enum::InputType::Button, // c = 'c',
	Enum::InputType::Button, // d = 'd',
	Enum::InputType::Button, // e = 'e',
	Enum::InputType::Button, // f = 'f',
	Enum::InputType::Button, // g = 'g',
	Enum::InputType::Button, // h = 'h',
	Enum::InputType::Button, // i = 'i',
	Enum::InputType::Button, // j = 'j',
	Enum::InputType::Button, // k = 'k',
	Enum::InputType::Button, // l = 'l',
	Enum::InputType::Button, // m = 'm',
	Enum::InputType::Button, // n = 'n',
	Enum::InputType::Button, // o = 'o',
	Enum::InputType::Button, // p = 'p',
	Enum::InputType::Button, // q = 'q',
	Enum::InputType::Button, // r = 'r',
	Enum::InputType::Button, // s = 's',
	Enum::InputType::Button, // t = 't',
	Enum::InputType::Button, // u = 'u',
	Enum::InputType::Button, // v = 'v',
	Enum::InputType::Button, // w = 'w',
	Enum::InputType::Button, // x = 'x',
	Enum::InputType::Button, // y = 'y',
	Enum::InputType::Button // z = 'z',
};

const char* inputNames[Enum::InputCode::Codes] = {
	"Mouse Left Button", // mouseLeft,
	"Mouse Right Button", // mouseRight,
	"Mouse Scroll Wheel Button", // mouseMiddle,
	"Mouse Scroll Wheel", // mouseWheel,
	"Mouse Position", // mousePosition,

	"Numpad 0", // numpadZero,
	"Numpad 1", // numpadOne,
	"Numpad 2", // numpadTwo,
	"Numpad 3", // numpadThree,
	"Numpad 4", // numpadFour,
	"Numpad 5", // numpadFive,
	"Numpad 6", // numpadSix,
	"Numpad 7", // numpadSeven,
	"Numpad 8", // numpadEight,
	"Numpad 9", // numpadNine,

	"Numpad Slash", // numpadSlash,
	"Numpad Astrisk", // numpadAstrisk,
	"Numpad Hyphen", // numpadHyphen,
	"Numpad Plus", // numpadPlus,
	"Numpad Enter", // numpadEnter,
	"Numpad Period", // numpadDelete,

	"Open Bracket", // bracketOpen,
	"Close Bracket", // bracketClose,
	"Backslash", // backslash,
	"Semicolon", // semicolon,
	"Apostrophe", // apostrophe,
	"Comma", // comma,
	"Period", // period,
	"Slash", // slash,
	"Grave Accent", // graveAccent,
	"Hyphen", // hyphen,
	"Equals", // equals,

	"Space", // space = ' ',

	"F1", // f1,
	"F2", // f2,
	"F3", // f3,
	"F4", // f4,
	"F5", // f5,
	"F6", // f6,
	"F7", // f7,
	"F8", // f8,
	"F9", // f9,
	"F10", // f10,
	"F11", // f11,
	"F12", // f12,

	"", // [45]
	"", // [46]
	"", // [47]

	"0", // zero = '0',
	"1", // one = '1',
	"2", // two = '2',
	"3", // three = '3',
	"4", // four = '4',
	"5", // five = '5',
	"6", // six = '6',
	"7", // seven = '7',
	"8", // eight = '8',
	"9", // nine = '9',

	"Escape", // escape,
	"Tab", // tab,
	"Caps Lock", // capsLock,
	"Left Shift", // leftShift,
	"Right Shift", // rightShift,
	"Left Control", // leftControl,
	"Right Control", // rightControl,
	"Left Alt", // leftAlt,
	"Right Alt", // rightAlt,
	"Enter", // enter,
	"Backspace", // backspace,
	"Insert", // insert,
	"Delete", // deleteButton,
	"Home", // home,
	"End", // end,
	"Page Up", // pageUp,
	"Page Down", // pageDown,
	"Num Lock", // numLock,

	"Up Arrow", // upArrow,
	"Down Arrow", // downArrow,
	"Left Arrow", // leftArrow,
	"Right Arrow", // rightArrow,

	"", // [80]
	"", // [81]
	"", // [82]
	"", // [83]
	"", // [84]
	"", // [85]
	"", // [86]
	"", // [87]
	"", // [88]
	"", // [89]
	"", // [90]
	"", // [91]
	"", // [92]
	"", // [93]
	"", // [94]
	"", // [95]
	"", // [96]

	"A", // a = 'a',
	"B", // b = 'b',
	"C", // c = 'c',
	"D", // d = 'd',
	"E", // e = 'e',
	"F", // f = 'f',
	"G", // g = 'g',
	"H", // h = 'h',
	"I", // i = 'i',
	"J", // j = 'j',
	"K", // k = 'k',
	"L", // l = 'l',
	"M", // m = 'm',
	"N", // n = 'n',
	"O", // o = 'o',
	"P", // p = 'p',
	"Q", // q = 'q',
	"R", // r = 'r',
	"S", // s = 's',
	"T", // t = 't',
	"U", // u = 'u',
	"V", // v = 'v',
	"W", // w = 'w',
	"X", // x = 'x',
	"Y", // y = 'y',
	"Z" // z = 'z',
};

const Enum::BoundDevice inputDevices[Enum::InputCode::Codes] = {
	Enum::BoundDevice::Mouse1, // mouseLeft,
	Enum::BoundDevice::Mouse1, // mouseRight,
	Enum::BoundDevice::Mouse1, // mouseMiddle,
	Enum::BoundDevice::Mouse1, // mouseWheel,
	Enum::BoundDevice::Mouse1, // mousePosition,
	
	Enum::BoundDevice::Input1, // numpadZero,
	Enum::BoundDevice::Input1, // numpadOne,
	Enum::BoundDevice::Input1, // numpadTwo,
	Enum::BoundDevice::Input1, // numpadThree,
	Enum::BoundDevice::Input1, // numpadFour,
	Enum::BoundDevice::Input1, // numpadFive,
	Enum::BoundDevice::Input1, // numpadSix,
	Enum::BoundDevice::Input1, // numpadSeven,
	Enum::BoundDevice::Input1, // numpadEight,
	Enum::BoundDevice::Input1, // numpadNine,
	
	Enum::BoundDevice::Input1, // numpadSlash,
	Enum::BoundDevice::Input1, // numpadAstrisk,
	Enum::BoundDevice::Input1, // numpadHyphen,
	Enum::BoundDevice::Input1, // numpadPlus,
	Enum::BoundDevice::Input1, // numpadEnter,
	Enum::BoundDevice::Input1, // numpadDelete,
	
	Enum::BoundDevice::Input1, // bracketOpen,
	Enum::BoundDevice::Input1, // bracketClose,
	Enum::BoundDevice::Input1, // backslash,
	Enum::BoundDevice::Input1, // semicolon,
	Enum::BoundDevice::Input1, // apostrophe,
	Enum::BoundDevice::Input1, // comma,
	Enum::BoundDevice::Input1, // period,
	Enum::BoundDevice::Input1, // slash,
	Enum::BoundDevice::Input1, // graveAccent,
	Enum::BoundDevice::Input1, // hyphen,
	Enum::BoundDevice::Input1, // equals,
	
	Enum::BoundDevice::Input1, // space = ' ',
					  
	Enum::BoundDevice::Input1, // f1,
	Enum::BoundDevice::Input1, // f2,
	Enum::BoundDevice::Input1, // f3,
	Enum::BoundDevice::Input1, // f4,
	Enum::BoundDevice::Input1, // f5,
	Enum::BoundDevice::Input1, // f6,
	Enum::BoundDevice::Input1, // f7,
	Enum::BoundDevice::Input1, // f8,
	Enum::BoundDevice::Input1, // f9,
	Enum::BoundDevice::Input1, // f10,
	Enum::BoundDevice::Input1, // f11,
	Enum::BoundDevice::Input1, // f12,
	
	Enum::BoundDevice::Any, // [45]
	Enum::BoundDevice::Any, // [46]
	Enum::BoundDevice::Any, // [47]
	
	Enum::BoundDevice::Input1, // zero = '0',
	Enum::BoundDevice::Input1, // one = '1',
	Enum::BoundDevice::Input1, // two = '2',
	Enum::BoundDevice::Input1, // three = '3',
	Enum::BoundDevice::Input1, // four = '4',
	Enum::BoundDevice::Input1, // five = '5',
	Enum::BoundDevice::Input1, // six = '6',
	Enum::BoundDevice::Input1, // seven = '7',
	Enum::BoundDevice::Input1, // eight = '8',
	Enum::BoundDevice::Input1, // nine = '9',
	
	Enum::BoundDevice::Input1, // escape,
	Enum::BoundDevice::Input1, // tab,
	Enum::BoundDevice::Input1, // capsLock,
	Enum::BoundDevice::Input1, // leftShift,
	Enum::BoundDevice::Input1, // rightShift,
	Enum::BoundDevice::Input1, // leftControl,
	Enum::BoundDevice::Input1, // rightControl,
	Enum::BoundDevice::Input1, // leftAlt,
	Enum::BoundDevice::Input1, // rightAlt,
	Enum::BoundDevice::Input1, // enter,
	Enum::BoundDevice::Input1, // backspace,
	Enum::BoundDevice::Input1, // insert,
	Enum::BoundDevice::Input1, // deleteButton,
	Enum::BoundDevice::Input1, // home,
	Enum::BoundDevice::Input1, // end,
	Enum::BoundDevice::Input1, // pageUp,
	Enum::BoundDevice::Input1, // pageDown,
	Enum::BoundDevice::Input1, // numLock,
	
	Enum::BoundDevice::Input1, // upArrow,
	Enum::BoundDevice::Input1, // downArrow,
	Enum::BoundDevice::Input1, // leftArrow,
	Enum::BoundDevice::Input1, // rightArrow,
	
	Enum::BoundDevice::Any, // [80]
	Enum::BoundDevice::Any, // [81]
	Enum::BoundDevice::Any, // [82]
	Enum::BoundDevice::Any, // [83]
	Enum::BoundDevice::Any, // [84]
	Enum::BoundDevice::Any, // [85]
	Enum::BoundDevice::Any, // [86]
	Enum::BoundDevice::Any, // [87]
	Enum::BoundDevice::Any, // [88]
	Enum::BoundDevice::Any, // [89]
	Enum::BoundDevice::Any, // [90]
	Enum::BoundDevice::Any, // [91]
	Enum::BoundDevice::Any, // [92]
	Enum::BoundDevice::Any, // [93]
	Enum::BoundDevice::Any, // [94]
	Enum::BoundDevice::Any, // [95]
	Enum::BoundDevice::Any, // [96]
	
	Enum::BoundDevice::Input1, // a = 'a',
	Enum::BoundDevice::Input1, // b = 'b',
	Enum::BoundDevice::Input1, // c = 'c',
	Enum::BoundDevice::Input1, // d = 'd',
	Enum::BoundDevice::Input1, // e = 'e',
	Enum::BoundDevice::Input1, // f = 'f',
	Enum::BoundDevice::Input1, // g = 'g',
	Enum::BoundDevice::Input1, // h = 'h',
	Enum::BoundDevice::Input1, // i = 'i',
	Enum::BoundDevice::Input1, // j = 'j',
	Enum::BoundDevice::Input1, // k = 'k',
	Enum::BoundDevice::Input1, // l = 'l',
	Enum::BoundDevice::Input1, // m = 'm',
	Enum::BoundDevice::Input1, // n = 'n',
	Enum::BoundDevice::Input1, // o = 'o',
	Enum::BoundDevice::Input1, // p = 'p',
	Enum::BoundDevice::Input1, // q = 'q',
	Enum::BoundDevice::Input1, // r = 'r',
	Enum::BoundDevice::Input1, // s = 's',
	Enum::BoundDevice::Input1, // t = 't',
	Enum::BoundDevice::Input1, // u = 'u',
	Enum::BoundDevice::Input1, // v = 'v',
	Enum::BoundDevice::Input1, // w = 'w',
	Enum::BoundDevice::Input1, // x = 'x',
	Enum::BoundDevice::Input1, // y = 'y',
	Enum::BoundDevice::Input1 // z = 'z',
};