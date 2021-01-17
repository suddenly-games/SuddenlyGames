#include "GameWindow.h"

namespace GraphicsEngine
{
	void GameWindow::Configure(Window* window)
	{
		ActiveWindow = window;
	}

	int GameWindow::GetRefreshRate()
	{
		if (ActiveWindow == nullptr)
			throw std::string("GameWindow.GetRefreshRate: Attempt to use unconfigured GameWindow.");

		return ActiveWindow->RefreshRate;
	}

	Vector3 GameWindow::GetResolution()
	{
		if (ActiveWindow == nullptr)
			throw std::string("GameWindow.GetRefreshRate: Attempt to use unconfigured GameWindow.");

		return ActiveWindow->Resolution;
	}

	void GameWindow::SetMousePosition(const Vector3& position)
	{
		if (ActiveWindow == nullptr)
			throw std::string("GameWindow.GetRefreshRate: Attempt to use unconfigured GameWindow.");

		ActiveWindow->SetMousePosition(position);
	}
}