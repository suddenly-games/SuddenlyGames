#pragma once

#include "Object.h"
#include "DeviceVector.h"

namespace GraphicsEngine
{
	class Font;

	class Text : public Engine::Object
	{
	public:
		DeviceAxis FontSize;
		std::weak_ptr<Font> FontData;

		void Initialize() {}
		void Update(float) {}

		void Draw() {}

		void SetText(const std::string& text);
		std::string GetText() const;

	private:
		float LastFontSize = 0;
		std::string Contents;
		Vector3 LastAbsoluteSize;

		Instantiable;

		Inherits_Class(Object);

		Reflected(Text);
	};
}