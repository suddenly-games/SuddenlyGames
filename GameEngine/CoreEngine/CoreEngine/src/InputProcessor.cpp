#include "InputProcessor.h"

InputProcessor::InputProcessor()
{
	Graphics::ActiveWindow->Input->GetInput(Enum::InputCode::MouseLeft).Began.Connect([this] (InputObject* input) -> bool
	{
		if (Focus != nullptr)
		{
			Focus->Mouse.Left.IsDown = true;
			LeftClickedFrame = Focus;
			Focus->Mouse.Left.Down.Fire(input + 0);
		}

		return true;
	});

	Graphics::ActiveWindow->Input->GetInput(Enum::InputCode::MouseLeft).Ended.Connect([this](InputObject* input) -> bool
	{
		if (LeftClickedFrame != nullptr)
		{
			LeftClickedFrame->Mouse.Left.IsDown = false;

			if (LeftClickedFrame == Focus)
				Focus->Mouse.Left.Up.Fire(input + 0);

			LeftClickedFrame = nullptr;
		}

		return true;
	});

	Graphics::ActiveWindow->Input->GetInput(Enum::InputCode::MouseMiddle).Began.Connect([this](InputObject* input) -> bool
	{
		if (Focus != nullptr)
		{
			Focus->Mouse.Wheel.IsDown = true;
			MiddleClickedFrame = Focus;
			Focus->Mouse.Wheel.Down.Fire(input + 0);
		}
		
		return true;
	});

	Graphics::ActiveWindow->Input->GetInput(Enum::InputCode::MouseMiddle).Ended.Connect([this](InputObject* input) -> bool
	{
		if (MiddleClickedFrame != nullptr)
		{
			MiddleClickedFrame->Mouse.Wheel.IsDown = false;

			if (MiddleClickedFrame == Focus)
				Focus->Mouse.Wheel.Up.Fire(input + 0);

			MiddleClickedFrame = nullptr;
		}

		return true;
	});

	Graphics::ActiveWindow->Input->GetInput(Enum::InputCode::MouseRight).Began.Connect([this](InputObject* input) -> bool
	{
		if (Focus != nullptr)
		{
			Focus->Mouse.Right.IsDown = true;
			RightClickedFrame = Focus;
			Focus->Mouse.Right.Down.Fire(input + 0);
		}

		return true;
	});

	Graphics::ActiveWindow->Input->GetInput(Enum::InputCode::MouseRight).Ended.Connect([this](InputObject* input) -> bool
	{
		if (RightClickedFrame != nullptr)
		{
			RightClickedFrame->Mouse.Right.IsDown = false;

			if (RightClickedFrame == Focus)
				Focus->Mouse.Right.Up.Fire(input + 0);

			RightClickedFrame = nullptr;
		}

		return true;
	});

	Graphics::ActiveWindow->Input->GetInput(Enum::InputCode::MouseWheel).Changed.Connect([this](InputObject* input) -> bool
	{
		if (Focus != nullptr)
			Focus->Mouse.Wheel.Moved.Fire(input + 0);

		return true;
	});
}

void InputProcessor::Process(FrameBase* frame)
{
	FrameBase* lastFocus = Focus;
	Focus = frame->GetFocusFrame(Graphics::ActiveWindow->GetMouse().Position);

	if (Focus != lastFocus)
	{
		if (Focus != nullptr)
			Focus->MouseEntered.Fire();

		if (lastFocus != nullptr)
			lastFocus->MouseLeft.Fire();
	}
}
