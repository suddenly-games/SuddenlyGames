#include "LuaInput.h"

#include "ObjectReflection.h"

namespace Engine
{
	Enum_Definition(InputCode,
		Document_Enum("");
		
		Enum_Item(MouseLeft);
		Enum_Item(MouseRight);
		Enum_Item(MouseMiddle);
		Enum_Item(MouseWheel);
		Enum_Item(MousePosition);

		Enum_Item(NumpadZero);
		Enum_Item(NumpadOne);
		Enum_Item(NumpadTwo);
		Enum_Item(NumpadThree);
		Enum_Item(NumpadFour);
		Enum_Item(NumpadFive);
		Enum_Item(NumpadSix);
		Enum_Item(NumpadSeven);
		Enum_Item(NumpadEight);
		Enum_Item(NumpadNine);

		Enum_Item(NumpadSlash);
		Enum_Item(NumpadAstrisk);
		Enum_Item(NumpadHyphen);
		Enum_Item(NumpadPlus);
		Enum_Item(NumpadEnter);
		Enum_Item(NumpadPeriod);

		Enum_Item(BracketOpen);
		Enum_Item(BracketClose);
		Enum_Item(Backslash);
		Enum_Item(Semicolon);
		Enum_Item(Apostrophe);
		Enum_Item(Comma);
		Enum_Item(Period);
		Enum_Item(Slash);
		Enum_Item(GraveAccent);
		Enum_Item(Hyphen);
		Enum_Item(Equals);

		Enum_Item(Space);

		Enum_Item(F1);
		Enum_Item(F2);
		Enum_Item(F3);
		Enum_Item(F4);
		Enum_Item(F5);
		Enum_Item(F6);
		Enum_Item(F7);
		Enum_Item(F8);
		Enum_Item(F9);
		Enum_Item(F10);
		Enum_Item(F11);
		Enum_Item(F12);

		Enum_Item(Zero);
		Enum_Item(One);
		Enum_Item(Two);
		Enum_Item(Three);
		Enum_Item(Four);
		Enum_Item(Five);
		Enum_Item(Six);
		Enum_Item(Seven);
		Enum_Item(Eight);
		Enum_Item(Nine);

		Enum_Item(Escape);
		Enum_Item(Tab);
		Enum_Item(CapsLock);
		Enum_Item(LeftShift);
		Enum_Item(RightShift);
		Enum_Item(LeftControl);
		Enum_Item(RightControl);
		Enum_Item(LeftAlt);
		Enum_Item(RightAlt);
		Enum_Item(Enter);
		Enum_Item(Backspace);
		Enum_Item(Insert);
		Enum_Item(DeleteButton);
		Enum_Item(Home);
		Enum_Item(End);
		Enum_Item(PageUp);
		Enum_Item(PageDown);
		Enum_Item(NumLock);

		Enum_Item(UpArrow);
		Enum_Item(DownArrow);
		Enum_Item(LeftArrow);
		Enum_Item(RightArrow);

		Enum_Item(A);
		Enum_Item(B);
		Enum_Item(C);
		Enum_Item(D);
		Enum_Item(E);
		Enum_Item(F);
		Enum_Item(G);
		Enum_Item(H);
		Enum_Item(I);
		Enum_Item(J);
		Enum_Item(K);
		Enum_Item(L);
		Enum_Item(M);
		Enum_Item(N);
		Enum_Item(O);
		Enum_Item(P);
		Enum_Item(Q);
		Enum_Item(R);
		Enum_Item(S);
		Enum_Item(T);
		Enum_Item(U);
		Enum_Item(V);
		Enum_Item(W);
		Enum_Item(X);
		Enum_Item(Y);
		Enum_Item(Z);

		Enum_Item(Codes);
	);

	Enum_Definition(InputType,
		Document_Enum("");
		
		Enum_Item(Button);
		Enum_Item(Point);
	);

	Enum_Definition(InputState,
		Document_Enum("");
		
		Enum_Item(Began);
		Enum_Item(Changed);
		Enum_Item(Ended);

		Enum_Item(Idle);
		Enum_Item(Active);

		Enum_Item(None);
	);

	Enum_Definition(BoundDevice,
		Document_Enum("");
		
		Enum_Item(Any);

		Enum_Item(Mouse1);

		Enum_Item(Input1);
	);
}