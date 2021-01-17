#pragma once

#include "Object.h"
#include "DeviceVector.h"

namespace GraphicsEngine
{
	class Font;

	class TextCanvas : public Engine::Object
	{
	public:
		bool Visible = true;
		DeviceAxis FontSize;
		std::weak_ptr<Font> FontData;

		void Initialize() {}
		void Update(float) {}

		void Draw() {}

		void SetText();
		std::string GetText() const;

	private:
		float LastFontSize = 0;
		std::string Text;
		Vector3 LastAbsoluteSize;

		Instantiable;

		Inherits_Class(Object);

		Reflected(TextCanvas);
	};
}