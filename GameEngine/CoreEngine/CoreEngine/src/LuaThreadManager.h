#pragma once

#include "Lua.h"

#include <functional>

namespace Engine
{
	namespace LuaThread
	{
		typedef std::function<int(lua_State*)> LuaCallback;

		void Initialize(lua_State* lua);
		void Update(lua_State* lua, float delta);
		int Spawn(const std::string& source, const std::string& name, LuaCallback initializeCallback = nullptr);
		void Kill(int threadID);
		bool Running(int threadID);
		bool Yielded(int threadID);
		bool Dead(int threadID);
		int GetData(lua_State* lua, int threadID);
		int GetThreadID(lua_State* lua);

		int CoroutineCreate(lua_State* lua);
		int CoroutineResume(lua_State* lua);
		int CoroutineYield(lua_State* lua);
		void ThreadEnded(int id);
	}
}
