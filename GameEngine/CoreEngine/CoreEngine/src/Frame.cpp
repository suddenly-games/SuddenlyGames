#include "Frame.h"

Frame::Frame(Frame* parent) : FrameBase(parent) {
	//mouseEntered.Connect<void>([this](const void*) {
	//	mouseHovering = true;
	//});
	//
	//mouseLeft.Connect<void>([this](const void*) {
	//	mouseHovering = false;
	//});
}

void Frame::DrawFrame() const
{
	const AppearanceData* currentAppearance = GetCurrentAppearance();

	Programs::Screen->blendTexture.Set(currentAppearance->BlendTexture);
	Programs::Screen->color.Set(currentAppearance->Color);
	Programs::Screen->textureColor.Set(currentAppearance->TextureColor);
	Programs::Screen->uvScale.Set(currentAppearance->UVScale.X, -currentAppearance->UVScale.Y);
	Programs::Screen->uvOffset.Set(currentAppearance->UVOffset.X, 1 - currentAppearance->UVOffset.Y);
	Programs::Screen->SetTexture(currentAppearance->Texture);
	Programs::Screen->SetClippingMask(GetClippingMask(), 1);

	Programs::Screen->transform.Set(GetTransformation());

	Programs::Screen->CoreMeshes.Square->Draw();
}

const AppearanceData* Frame::GetCurrentAppearance() const
{
	return &Appearance;
}
