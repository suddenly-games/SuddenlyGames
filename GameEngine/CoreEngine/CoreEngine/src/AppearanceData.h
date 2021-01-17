#pragma once

#include "RGBA.h"
#include "Vector3.h"
#include "Texture.h"

struct AppearanceData
{
	bool BlendTexture = true;
	RGBA Color;
	RGBA TextureColor;
	Vector3 UVScale = Vector3(1, -1);
	Vector3 UVOffset = Vector3(0, 1);
	Handle<GraphicsEngine::Texture> Texture;

	AppearanceData() {}
};
