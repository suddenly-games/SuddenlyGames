#pragma once

#include "FrameBase.h"

class DraggableFrame : public FrameBase
{
public:
	FrameBase* Target = nullptr;

	Event<InputObject*> Dragged;

	DraggableFrame();

	bool IsDragging() const;

private:
	bool Dragging = false;
	Vector3 InitialLocalPoint;
	Vector3 InitialPosition;
};
