#pragma once

#include <memory>

namespace Engine
{
	class GameEngine;

	const std::shared_ptr<GameEngine>& Root();

	void InitializeEngine();
	void CleanEngine();
}
