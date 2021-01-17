#include "Engine.h"

#include "Object.h"

namespace Engine
{
	struct Initializer
	{
		std::shared_ptr<Object> Root;
	};
	
	Initializer* initializer = nullptr;

	const std::shared_ptr<Object>& Root()
	{
		return initializer->Root;
	}

	void InitializeEngine()
	{
		initializer = new Initializer();

		initializer->Root = Create<Object>();

		initializer->Root->Name = "Engine";
	}

	void CleanEngine()
	{
		delete initializer;
	}
}
