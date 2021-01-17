#pragma once

#include <memory>

#include "Lua.h"
#include "IdentifierHeap.h"

class ReflectionData;
struct lua_State;

namespace Engine
{
	class LuaData
	{
	public:
		struct LuaTypeEnum
		{
			enum LuaType
			{
				Object,
				Enum,
				Type,

				None
			};
		};

		typedef LuaTypeEnum::LuaType LuaType;

		LuaType Type = LuaType::None;
		void* Data = nullptr;
		std::shared_ptr<ObjectBase> Reference;
		const ReflectionData* Meta = nullptr;

		LuaData(const LuaType& type) : Type(type)
		{

		}
	};

	typedef std::function<int(lua_State * lua)> LuaCallback;
	typedef IDHeap<LuaCallback> LuaFunctionHeap;
	typedef IDHeap<lua_State*> LuaThreadHeap;

	extern LuaFunctionHeap LuaFunctions;
	extern LuaThreadHeap LuaThreads;
	void BindLua(lua_State* lua);
	void UpdateLua(lua_State* lua, float delta);
	int LuaQueueThread(lua_State* lua);
	int LuaGameTime(lua_State* lua);
	void LuaQueueEventCall(const LuaCallback& callback);
	void MakeLuaReference(lua_State* lua, int id);
	void MakeLuaTypeReference(lua_State* lua, const ReflectionData* type, void* data = nullptr);
}
