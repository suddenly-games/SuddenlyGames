#pragma once

#include "IdentifierHeap.h"
#include "Lua.h"
#include "Object.h"

namespace Engine
{
	class LuaSource;

	class LuaScript : public Object
	{
	public:
		struct ScriptStatusEnum
		{
			enum ScriptStatus
			{
				Idle,
				Running,
				Yielded,
				Dead
			};
		};

		typedef ScriptStatusEnum::ScriptStatus ScriptStatus;

		void Initialize();
		void Update(float delta);

		~LuaScript();

		std::shared_ptr<LuaSource> GetSource() const;
		const std::string& GetSourceCode() const;
		int GetVersion() const;
		bool UpToDate() const;
		bool AutoRunEnabled() const;
		ScriptStatus GetStatus() const;
		void SetSource(const std::shared_ptr<LuaSource>& source);
		void SetSource(const std::string& source);
		void SetAutoRun(bool mode);
		void Run();
		void Stop();
		void Reload();
		int GetData(lua_State* lua);

	private:
		std::string Path;
		std::string Source;
		std::weak_ptr<LuaSource> SourceObject;
		int LoadedVersion = -1;
		int ThreadID = -1;
		bool SourceChanged = false;
		bool DataLoaded = false;
		bool AutoRuns = false;

		Instantiable;

		Inherits_Class(Object);

		Reflected(LuaScript);
	};

	//Class_Inherits(LuaScript, Object);
	Declare_Enum(ScriptStatus);
}

namespace Enum
{
	typedef Engine::LuaScript::ScriptStatus ScriptStatus;
}
