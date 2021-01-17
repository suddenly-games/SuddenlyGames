#pragma once

#include "Matrix3.h"
#include "Graphics.h"
#include "RGBA.h"
#include "Mesh.h"
#include "Font.h"
#include "FrameBase.h"

class TextData
{
private:
	struct CharacterData
	{
		Vector3 Size;
		Vector3 Position;
		Vector3 UVScale;
		Vector3 UVOffset;
		Matrix3 Transformation;
	};

public:
	RGBA Color;

	void Draw(const Vector3& position) const;
	void Draw(const Vector3& position, const Vector3& resolution) const;
	void Draw(const Vector3& position, const FrameBase* frame) const;
	void SetText(const std::string& newText);
	void SetFontSize(float size);
	void SetMaxBounds(const Vector3& bounds);
	void SetFont(Handle<GraphicsEngine::Font> newFont);
	const std::string& GetText() const;
	float GetFontSize() const;
	const Vector3& GetTextBounds() const;
	const Vector3& GetMaxBounds() const;
	Handle<GraphicsEngine::Font> GetFont() const;
	void UpdateTransformation(const Vector3& position, const FrameBase* frame);

private:
	typedef std::vector<CharacterData> TextDataVector;

	std::string Text;
	float FontSize = 10;
	Vector3 TextBounds;
	Vector3 MaxBounds;
	Handle<GraphicsEngine::Font> FontType;
	TextDataVector RawTextData;
	bool TransformationSet = false;

	void UpdateTextData();
};
