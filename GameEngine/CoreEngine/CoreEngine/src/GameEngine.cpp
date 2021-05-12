#include "GameEngine.h"

namespace Engine
{
	int ProcessArguments::GetArguments() const
	{
		return int(Arguments.size());
	}

	std::string ProcessArguments::GetArgument(int index) const
	{
		return Arguments[index];
	}

	bool ProcessArguments::HasArgumentFlag(const std::string& name) const
	{
		return ArgumentFlags.find(name) != ArgumentFlags.end();
	}

	std::string ProcessArguments::GetArgumentValue(const std::string& name) const
	{
		FlagMap::const_iterator value = ArgumentFlags.find(name);

		if (value != ArgumentFlags.end())
			return value->second;

		return "";
	}

	void ProcessArguments::PushArgument(const std::string& argument)
	{
		Arguments.push_back(argument);

		if (argument.size() >= 2 && argument[0] == '-')
		{
			int seperator = 1;
			
			for (seperator = 1; seperator < int(argument.size()) && argument[seperator] != '='; ++seperator);

			if (seperator == int(argument.size()))
				InsertArgumentFlag(argument.substr(1, argument.size() - 1));
			else
				InsertArgument(argument.substr(1, seperator - 1), argument.substr(seperator + 1));
		}
	}

	void ProcessArguments::InsertArgumentFlag(const std::string& name)
	{
		ArgumentFlags[name] = "";
	}

	void ProcessArguments::InsertArgument(const std::string& name, const std::string& value)
	{
		ArgumentFlags[name] = value;
	}

	int GameEngine::GetArguments() const
	{
		return Arguments.GetArguments();
	}

	std::string GameEngine::GetArgument(int index) const
	{
		return Arguments.GetArgument(index);
	}

	bool GameEngine::HasArgumentFlag(const std::string& name) const
	{
		return Arguments.HasArgumentFlag(name);
	}

	std::string GameEngine::GetArgumentValue(const std::string& name) const
	{
		return Arguments.GetArgumentValue(name);
	}

	void GameEngine::Configure(int argc, const char* const* argv)
	{
		for (int i = StartArgument; i < argc; ++i)
			Arguments.PushArgument(argv[i]);
	}
}