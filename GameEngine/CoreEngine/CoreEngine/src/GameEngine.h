#pragma once

#include <vector>
#include <map>

#include "Object.h"

#ifdef CreateWindow
#undef CreateWindow
#endif

namespace Engine
{
	struct ProcessArguments
	{
		ProcessArguments() {}

		int GetArguments() const;
		std::string GetArgument(int index) const;
		bool HasArgumentFlag(const std::string& name) const;
		std::string GetArgumentValue(const std::string& name) const;

		void PushArgument(const std::string& argument);
		void InsertArgumentFlag(const std::string& name);
		void InsertArgument(const std::string& name, const std::string& value);

	private:
		typedef std::map<std::string, std::string> FlagMap;

		std::vector<std::string> Arguments;
		FlagMap ArgumentFlags;

		Base_Class;

		Reflected_Type(ProcessArguments);
	};

	class GameWindow;

	Define_Value_Type(ProcessArguments);

	class GameEngine : public Object
	{
	public:
		virtual ~GameEngine() {}

		void Initialize() {}
		void Update(float delta) {}

		int GetArguments() const;
		std::string GetArgument(int index) const;
		bool HasArgumentFlag(const std::string& name) const;
		std::string GetArgumentValue(const std::string& name) const;

		int SpawnProcess(const std::string& scriptPath, const ProcessArguments& arguments = ProcessArguments()) const { return -1; }

		std::shared_ptr<GameWindow> CreateWindow(const std::string& name, const std::string& title) const { return nullptr; }
		void CreateConsole(const std::string& name, const std::string& title) const { return; }

		void Configure(int argc, const char* const* argv);

	private:
		static const int StartArgument = 1;

		ProcessArguments Arguments;

		Instantiable;

		Inherits_Class(Object);

		Reflected(GameEngine);
	};

	//Class_Inherits(Environments, Object);
}