#pragma once

#include "RGBA.h"
#include "Mesh.h"
#include "Texture.h"
#include "Graphics.h"
#include "FrameBase.h"
#include "AppearanceData.h"

class Frame : public FrameBase
{
public:
	AppearanceData Appearance;

	Frame(Frame* parent = nullptr);

	void DrawFrame() const;
	virtual const AppearanceData* GetCurrentAppearance() const;
};






