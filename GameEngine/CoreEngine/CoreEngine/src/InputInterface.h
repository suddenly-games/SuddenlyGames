#pragma once

#include "LuaInput.h"
#include "Object.h"

namespace GraphicsEngine
{
	class InputInterface : public Engine::Object
	{
	public:
		bool Enabled = true;

		std::weak_ptr<Object> UpConnection;
		std::weak_ptr<Object> DownConnection;
		std::weak_ptr<Object> LeftConnection;
		std::weak_ptr<Object> RightConnection;

		~InputInterface();

		void Initialize();
		void Update(float);

		void Process();

		Event<std::shared_ptr<Engine::InputData>> Began;
		Event<std::shared_ptr<Engine::InputData>> Changed;
		Event<std::shared_ptr<Engine::InputData>> Ended;

	private:

		Instantiable;

		Inherits_Class(Object);

		Reflected(InputInterface);
	};
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::InputInterface, Object);
}