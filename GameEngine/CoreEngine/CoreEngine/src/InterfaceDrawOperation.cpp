#include "InterfaceDrawOperation.h"

#include "Graphics.h"

namespace GraphicsEngine
{
	void InterfaceDrawOperation::Render()
	{
		glDisable(GL_DEPTH_TEST);

		auto currentScreen = CurrentScreen.lock();

		if (currentScreen != nullptr && currentScreen->Visible)
		{
			Programs::ClippingMask->Use();
			
			currentScreen->Draw(true);
			
			Programs::Screen->Use();
			
			currentScreen->Draw(false);
		}
	}
}