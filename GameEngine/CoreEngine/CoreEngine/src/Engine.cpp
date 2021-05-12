#include "Engine.h"

#include "GameEngine.h"

namespace Engine
{
	struct Initializer
	{
		std::shared_ptr<GameEngine> Root;
	};
	
	Initializer* initializer = nullptr;

	const std::shared_ptr<GameEngine>& Root()
	{
		return initializer->Root;
	}

	void InitializeEngine()
	{
		initializer = new Initializer();

		initializer->Root = Create<GameEngine>();
	}

	void CleanEngine()
	{
		delete initializer;
	}
}
