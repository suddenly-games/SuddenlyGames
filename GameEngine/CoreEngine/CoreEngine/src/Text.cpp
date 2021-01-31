#include "Text.h"

namespace GraphicsEngine
{
	void Text::SetText(const std::string& text)
	{
		Contents = text;
	}

	std::string Text::GetText() const
	{
		return Contents;
	}
}