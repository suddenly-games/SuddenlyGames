#include "DraggableFrame.h"

#include "Graphics.h"

DraggableFrame::DraggableFrame()
{
	Mouse.Left.Down.Connect([this](InputObject*) -> bool
	{
		Dragging = true;

		FrameBase* currentTarget = Target;

		if (Target == nullptr)
			currentTarget = this;

		InitialLocalPoint = currentTarget->GetParent()->GetLocalPosition(Graphics::ActiveWindow->GetMouse().Position);
		InitialPosition = currentTarget->GetPosition().Calculate(Vector3(), currentTarget->GetParent()->GetAbsoluteSize());

		return true;
	});

	Graphics::ActiveWindow->Input->GetInput(Enum::InputCode::MouseLeft).Ended.Connect([this](InputObject*) -> bool
	{
		Dragging = false;

		return true;
	});

	Graphics::ActiveWindow->Input->GetInput(Enum::InputCode::MousePosition).Changed.Connect([this](InputObject* object) -> bool
	{
		if (Dragging)
		{
			Vector3 delta = object->GetDelta();

			FrameBase* currentTarget = Target;

			if (Target == nullptr)
				currentTarget = this;

			Vector3 newLocalPoint = currentTarget->GetParent()->GetLocalPosition(Graphics::ActiveWindow->GetMouse().Position);
			Vector3 currentPosition = currentTarget->GetPosition().Calculate(Vector3(), currentTarget->GetParent()->GetAbsoluteSize());
			Vector3 offset = newLocalPoint - InitialLocalPoint + InitialPosition - currentPosition;

			DeviceVector position = currentTarget->GetPosition();

			position.X.Offset += offset.X;//delta.X;
			position.Y.Offset += offset.Y;//delta.Y;

			currentTarget->SetPosition(position);

			Dragged.Fire(object + 0);
		}

		return true;
	});
}

bool DraggableFrame::IsDragging() const
{
	return Dragging;
}
