#include "TextFrame.h"

TextFrame::TextFrame()
{
	InputEnabled = false;
}

void TextFrame::DrawFrame() const
{
	Vector3 size = GetAbsoluteSize();
	Vector3 bounds = Text.GetTextBounds();

	float x = float(TextAlignmentX) * 0.5f * (size.X - bounds.X);
	float y = float(TextAlignmentY) * 0.5f * (size.Y - bounds.Y);

	Text.Draw(Vector3(x, y), this);
}

void TextFrame::UpdateFrame()
{
	Text.SetFontSize(FontSize.Calculate(0, GetAbsoluteSize().Y));

	Vector3 size = GetAbsoluteSize();
	Vector3 bounds = Text.GetTextBounds();

	float x = float(TextAlignmentX) * 0.5f * (size.X - bounds.X);
	float y = float(TextAlignmentY) * 0.5f * (size.Y - bounds.Y);

	//text.UpdateTransformation(Vector3(x, y), this);
}
