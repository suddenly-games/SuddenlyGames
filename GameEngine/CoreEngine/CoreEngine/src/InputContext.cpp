#include "InputContext.h"

namespace GraphicsEngine
{
	std::weak_ptr<InputContext> InputContext::CurrentContext = std::weak_ptr<InputContext>();

	void InputContext::Update(float)
	{

	}
}