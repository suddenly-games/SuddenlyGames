#pragma once

#include <memory>

namespace Engine
{
	class Object;

	const std::shared_ptr<Object>& Root();

	void InitializeEngine();
	void CleanEngine();
}
