
#include <iostream>
#include <forward_list>
extern "C" {
#include <Windows.h>
}

#include "Object.h"
#include "GameEngine.h"
#include "ObjectRegistration.h"
#include "Lua.h"
#include "LuaBinding.h"
#include "Graphics.h"
#include "Scene.h"
#include "FrameBase.h"
#include "InputProcessor.h"
#include "GlowingSceneOperation.h"
#include "Engine.h"
#include "GameWindow.h"
#include "LuaInput.h"
#include "LuaSource.h"
#include "LuaScript.h"
#include "VoxelTriangulation.h"

// Add NoLua reflection tag
// Add Object::Clone(bool deep = true)

enum ab { c };

int main(int argc, char* argv[])
{
  // Initialization
	try
	{
		Engine::InitializeObjectTypes();
		Engine::InitializeEngine();
		Graphics::Initialize();

		Engine::Root()->Configure(argc, &argv[0]);
	}
	catch (std::string& error)
	{
		std::cout << error << std::endl;
		OutputDebugString(error.c_str());
		abort();
	}

  // Engine Start
	{
		Window& window = *Graphics::System->ActiveWindow;

		Lua lua;

		Engine::BindLua(lua.GetState());

		window.Update();

		std::shared_ptr<GraphicsEngine::GameWindow> gameWindow = Engine::Create<GraphicsEngine::GameWindow>();

		gameWindow->Configure(&window);
		gameWindow->SetParent(Engine::Root());

		std::shared_ptr<Engine::UserInput> userInput = Engine::Create<Engine::UserInput>();
		 
		userInput->Configure(window.Input);
		userInput->SetParent(gameWindow);

		Engine::VoxelTriangulation::Initialize();

		{
			std::string path;

			if (argc > 1)
				path = argv[1];
			else
				path = "./game.lua";

			std::shared_ptr<Engine::LuaSource> source = Engine::Create<Engine::LuaSource>();
			source->Name = "GameScriptSource";
			source->LoadSource(path);
			
			std::shared_ptr<Engine::LuaScript> script = Engine::Create<Engine::LuaScript>();
			script->Name = "GameScript";
			script->SetSource(source);
			script->SetParent(Engine::Root());

			source->SetParent(script);

			script->Run();
		}

		Engine::UpdateLua(lua.GetState(), 0);

		// Game Loop
		while (!window.IsClosed())
		{
			float delta = window.Update();

			if (delta > 1.0f / 30)
				delta = 1.0f / 30;

			TaskScheduler::Update(delta);

			std::shared_ptr<Engine::ObjectBase>(Engine::Root())->UpdateBase(delta);

			Engine::UpdateLua(lua.GetState(), delta);

			GraphicsEngine::RenderOperation::RenderOperations();

			window.Swap();

			//Engine::CollectGarbage();
		}
	}

  // Shutdown
	try
	{
		Engine::CleanEngine();
		//Engine::CollectGarbage();

		Graphics::Clean();
		//Engine::CleanHandles();
		ReflectionData::CleanUp();
	}
	catch (std::string& error)
	{
		std::cout << error << std::endl;
		OutputDebugString(error.c_str());
		abort();
	}

	return 0;
}
