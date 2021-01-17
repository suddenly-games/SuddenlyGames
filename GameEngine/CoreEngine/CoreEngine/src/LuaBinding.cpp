#include "LuaBinding.h"

#include <iostream>

#include "Lua.h"
#include "Enum.h"
#include "Object.h"
#include "Engine.h"
#include "LuaObjectMetatable.h"
#include "TaskScheduler.h"
#include "Graphics.h"
#include "LuaJson.h"
#include "LuaThreadManager.h"
#include <iostream>

#undef Archivable

namespace Engine
{
	typedef LuaData::LuaType LuaType;

	int typeof(lua_State* lua)
	{
		if (lua_isuserdata(lua, 1))
		{
			LuaData* data = (LuaData*)lua_topointer(lua, 1);

			if (data->Meta)
				lua_pushstring(lua, data->Meta->Name.c_str());
			else
				lua_pushstring(lua, "userdata");

			return 1;
		}
		else
		{
			lua_getglobal(lua, "type");
			lua_pushvalue(lua, 1);
			lua_call(lua, 1, 1);
		}

		return 1;
	}

	void BindLua(lua_State* lua)
	{
		{
			luaL_newmetatable(lua, "objectMetatable");

			int meta = lua_gettop(lua);

			luaL_setfuncs(lua, ObjectMetatable::Metatable, 0);

			lua_pushstring(lua, "__metatable");
			lua_pushstring(lua, "This metatable is protected");

			lua_settable(lua, meta);
		}

		const EnumType::EnumTypeMap& enumTypes = EnumType::GetTypes();

		lua_createtable(lua, 0, int(enumTypes.size()));

		int enumIndex = lua_gettop(lua);

		for (EnumType::EnumTypeMap::const_iterator i = enumTypes.begin(); i != enumTypes.end(); ++i)
		{
			lua_pushstring(lua, i->second.Name.c_str());
			lua_createtable(lua, 0, int(i->second.Items.size()));

			int itemIndex = lua_gettop(lua);

			for (EnumType::EnumItemMap::const_iterator j = i->second.Items.begin(); j != i->second.Items.end(); ++j)
			{
				lua_pushstring(lua, j->second.Name.c_str());

				LuaData* newData = (LuaData*)lua_newuserdata(lua, sizeof(LuaData));

				new (newData) LuaData(LuaType::Enum);

				newData->Meta = ReflectionData::GetType("Enum." + i->second.Name + "." + j->second.Name);

				luaL_getmetatable(lua, "objectMetatable");
				lua_setmetatable(lua, -2);

				lua_settable(lua, itemIndex);
			}

			lua_settable(lua, enumIndex);
		}

		lua_setglobal(lua, "Enum");

		lua_createtable(lua, 0, 1);
		lua_pushstring(lua, "traceback");
		lua_pushcfunction(lua, [] (lua_State* lua) -> int
		{
			luaL_traceback(lua, lua, 0, 0);

			return 1;
		});
		lua_settable(lua, -3);
		lua_setglobal(lua, "debug");

		{
			lua_createtable(lua, 0, 0);

			int table = lua_gettop(lua);

			{
				lua_pushvalue(lua, lua_gettop(lua));

				int table = lua_gettop(lua);

				lua_createtable(lua, 0, 1);

				int meta = lua_gettop(lua);

				lua_pushstring(lua, "__mode");
				lua_pushstring(lua, "v");
				lua_settable(lua, meta);
				lua_setmetatable(lua, table);
			}

			lua_pushstring(lua, "Objects");
			lua_pushvalue(lua, table);

			lua_settable(lua, LUA_REGISTRYINDEX);
		}

		lua_settop(lua, 0);

		{
			const ReflectionData::DataVector& reflected = ReflectionData::GetTypes();

			int temp = lua_gettop(lua);

			lua_createtable(lua, 0, int(reflected.size()));
		
			int table = lua_gettop(lua);
		
			for (int i = 0; i < int(reflected.size()); ++i)
			{
				const ReflectionData* type = reflected[i];

				if (type->IsEnum)
					continue;
			
				lua_pushstring(lua, type->Name.c_str());temp = lua_gettop(lua);
				lua_createtable(lua, 0, 10);temp = lua_gettop(lua);
			
				int typeIndex = lua_gettop(lua);
			
				lua_pushstring(lua, "ID");temp = lua_gettop(lua);
				lua_pushnumber(lua, lua_Number(type->ID));temp = lua_gettop(lua);
				lua_settable(lua, typeIndex);temp = lua_gettop(lua);
			
				lua_pushstring(lua, "IsClass");temp = lua_gettop(lua);
				lua_pushboolean(lua, type->IsClass);temp = lua_gettop(lua);
				lua_settable(lua, typeIndex);temp = lua_gettop(lua);
			
				lua_pushstring(lua, "IsLibrary");temp = lua_gettop(lua);
				lua_pushboolean(lua, type->IsLibrary);temp = lua_gettop(lua);
				lua_settable(lua, typeIndex);temp = lua_gettop(lua);
			
				//lua_pushstring(lua, "IsEnum");temp = lua_gettop(lua);
				//lua_pushboolean(lua, type->IsEnum);temp = lua_gettop(lua);
				//lua_settable(lua, typeIndex);temp = lua_gettop(lua);
			
				lua_pushstring(lua, "UsesReferences");temp = lua_gettop(lua);
				lua_pushboolean(lua, type->UseReferences);temp = lua_gettop(lua);
				lua_settable(lua, typeIndex);temp = lua_gettop(lua);
			
				lua_pushstring(lua, "Name");temp = lua_gettop(lua);
				lua_pushstring(lua, type->Name.c_str());temp = lua_gettop(lua);
				lua_settable(lua, typeIndex);temp = lua_gettop(lua);
			
				lua_pushstring(lua, "Description");temp = lua_gettop(lua);
				lua_pushstring(lua, type->Description.c_str());temp = lua_gettop(lua);
				lua_settable(lua, typeIndex);temp = lua_gettop(lua);
			
				lua_pushstring(lua, "Members");temp = lua_gettop(lua);
				lua_createtable(lua, 0, int(type->Members.size()));temp = lua_gettop(lua);
				
				int membersIndex = lua_gettop(lua);
				
				for (ReflectionData::MemberMap::const_iterator j = type->Members.begin(); j != type->Members.end(); ++j)
				{
					const ReflectionData::Member* member = j->second;
				
					lua_pushstring(lua, member->Name.c_str());temp = lua_gettop(lua);
					lua_createtable(lua, 0, 5);temp = lua_gettop(lua);
					
					int memberIndex = lua_gettop(lua);
					
					lua_pushstring(lua, "IsStatic");temp = lua_gettop(lua);
					lua_pushboolean(lua, member->IsStatic);temp = lua_gettop(lua);
					lua_settable(lua, memberIndex);temp = lua_gettop(lua);
					
					lua_pushstring(lua, "Archivable");
					lua_pushboolean(lua, member->Archivable);
					lua_settable(lua, memberIndex);
					
					lua_pushstring(lua, "ReadOnly");
					lua_pushboolean(lua, member->ReadOnly);
					lua_settable(lua, memberIndex);

					lua_pushstring(lua, "Description");
					lua_pushstring(lua, member->Description.c_str());
					lua_settable(lua, memberIndex);
					
					lua_settable(lua, membersIndex);temp = lua_gettop(lua);
				}
				
				lua_settable(lua, typeIndex);temp = lua_gettop(lua);
			
				lua_pushstring(lua, "Functions");
				lua_createtable(lua, 0, int(type->Functions.size()));
				
				int functionsIndex = lua_gettop(lua);
				
				for (ReflectionData::FunctionMap::const_iterator j = type->Functions.begin(); j != type->Functions.end(); ++j)
				{
					const ReflectionData::Function* function = j->second;
				
					lua_pushstring(lua, function->Name.c_str());
					lua_createtable(lua, function->GetOverloads(), 0);

					lua_pushstring(lua, "Parent");
					lua_pushvalue(lua, typeIndex);
					lua_settable(lua, functionsIndex);
				
					int functionIndex = lua_gettop(lua);
				
					for (int k = 0; k < function->GetOverloads(); ++k)
					{
						const ReflectionData::FunctionOverloadData* overload = function->Overloads[k];
				
						lua_pushnumber(lua, lua_Number(k + 1));
						lua_createtable(lua, 0, 4);
				
						int overloadIndex = lua_gettop(lua);
				
						lua_pushstring(lua, "IsStatic");
						lua_pushboolean(lua, overload->IsStatic);
						lua_settable(lua, overloadIndex);
				
						lua_pushstring(lua, "ReturnValueDescription");
						lua_pushstring(lua, overload->ReturnValueDescription.c_str());
						lua_settable(lua, overloadIndex);
				
						lua_pushstring(lua, "Parameters");
						lua_createtable(lua, overload->GetParameters(), 0);

						lua_pushstring(lua, "Parent");
						lua_pushvalue(lua, functionIndex);
						lua_settable(lua, overloadIndex);

						lua_pushstring(lua, "ParentType");
						lua_pushstring(lua, overload->ParentName.c_str());
						lua_settable(lua, overloadIndex);
				
						int parametersIndex = lua_gettop(lua);
				
						for (int l = 0; l < overload->GetParameters(); ++l)
						{
							const ReflectionData::FunctionOverloadData::ParameterData& parameter = overload->Parameters[l];
				
							lua_pushnumber(lua, lua_Number(l + 1));
							lua_createtable(lua, 0, 4);
				
							int parameterIndex = lua_gettop(lua);
				
							lua_pushstring(lua, "Description");
							lua_pushstring(lua, parameter.Description.c_str());
							lua_settable(lua, parameterIndex);
				
							lua_pushstring(lua, "Name");
							lua_pushstring(lua, parameter.Name.c_str());
							lua_settable(lua, parameterIndex);

							lua_pushstring(lua, "Parent");
							lua_pushvalue(lua, overloadIndex);
							lua_settable(lua, parameterIndex);
				
							if (parameter.HasDefaultValue)
							{
								lua_pushstring(lua, "DefaultValue");
								lua_pushstring(lua, parameter.DefaultValue.c_str());
								lua_settable(lua, parameterIndex);
							}
				
							lua_settable(lua, parametersIndex);
						}
				
						lua_settable(lua, overloadIndex);
				
						lua_settable(lua, functionIndex);
					}
				
					lua_settable(lua, functionsIndex);
				}

				lua_settable(lua, typeIndex);temp = lua_gettop(lua);

				lua_settable(lua, table);temp = lua_gettop(lua);
			}

			for (int i = 0; i < int(reflected.size()); ++i)
			{
				const ReflectionData* type = reflected[i];

				if (type->IsEnum)
					continue;

				lua_pushstring(lua, type->Name.c_str());temp = lua_gettop(lua);
				lua_gettable(lua, table);

				int typeIndex = lua_gettop(lua);

				if (type->ParentData != nullptr)
				{
					lua_pushstring(lua, "Parent");
					lua_pushstring(lua, type->ParentData->Name.c_str());
					lua_gettable(lua, table);
					lua_settable(lua, typeIndex);
				}

				lua_pushstring(lua, "Members");temp = lua_gettop(lua);
				lua_gettable(lua, typeIndex);

				int membersIndex = lua_gettop(lua);

				for (ReflectionData::MemberMap::const_iterator j = type->Members.begin(); j != type->Members.end(); ++j)
				{
					const ReflectionData::Member* member = j->second;

					lua_pushstring(lua, member->Name.c_str());temp = lua_gettop(lua);
					lua_gettable(lua, membersIndex);

					int memberIndex = lua_gettop(lua);

					lua_pushstring(lua, "Type");

					if (member->Type->IsEnum)
					{
						lua_createtable(lua, 0, 2);

						int enumIndex = lua_gettop(lua);

						lua_pushstring(lua, "IsEnum");
						lua_pushboolean(lua, true);
						lua_settable(lua, enumIndex);

						lua_pushstring(lua, "Name");
						lua_pushstring(lua, "Enum.");
						lua_pushstring(lua, member->Type->Name.c_str());
						lua_concat(lua, 2);
						lua_settable(lua, enumIndex);

						lua_settable(lua, memberIndex);
					}
					else
					{
						lua_pushstring(lua, member->Type->Name.c_str());
						lua_gettable(lua, table);
						lua_settable(lua, memberIndex);
					}

					lua_pop(lua, 1);
				}

				lua_pop(lua, 1);

				lua_pushstring(lua, "Functions");
				lua_gettable(lua, typeIndex);

				int functionsIndex = lua_gettop(lua);

				for (ReflectionData::FunctionMap::const_iterator j = type->Functions.begin(); j != type->Functions.end(); ++j)
				{
					const ReflectionData::Function* function = j->second;

					lua_pushstring(lua, function->Name.c_str());
					lua_gettable(lua, functionsIndex);

					int functionIndex = lua_gettop(lua);

					for (int k = 0; k < function->GetOverloads(); ++k)
					{
						const ReflectionData::FunctionOverloadData* overload = function->Overloads[k];

						lua_pushnumber(lua, lua_Number(k + 1));
						lua_gettable(lua, functionIndex);

						int overloadIndex = lua_gettop(lua);

						lua_pushstring(lua, "Parameters");
						lua_gettable(lua, overloadIndex);

						int parametersIndex = lua_gettop(lua);

						for (int l = 0; l < overload->GetParameters(); ++l)
						{
							const ReflectionData::FunctionOverloadData::ParameterData& parameter = overload->Parameters[l];

							lua_pushnumber(lua, lua_Number(l + 1));
							lua_gettable(lua, parametersIndex);

							int parameterIndex = lua_gettop(lua);

							lua_pushstring(lua, "Type");

							if (parameter.Type->IsEnum)
							{
								lua_createtable(lua, 0, 2);

								int enumIndex = lua_gettop(lua);

								lua_pushstring(lua, "IsEnum");
								lua_pushboolean(lua, true);
								lua_settable(lua, enumIndex);

								lua_pushstring(lua, "Name");
								lua_pushstring(lua, "Enum.");
								lua_pushstring(lua, parameter.Type->Name.c_str());
								lua_concat(lua, 2);
								lua_settable(lua, enumIndex);

								lua_settable(lua, parameterIndex);
							}
							else
							{
								lua_pushstring(lua, parameter.Type->Name.c_str());
								lua_gettable(lua, table);
								lua_settable(lua, parameterIndex);
							}

							lua_pop(lua, 1);
						}

						lua_pop(lua, 2);
					}

					lua_pop(lua, 1);
				}

				lua_pop(lua, 2);
			}
		
			lua_setglobal(lua, "Meta");
			//lua_settable(lua, LUA_REGISTRYINDEX);temp = lua_gettop(lua);
		}

		luaL_newmetatable(lua, "objectConstructorMetatable");
		luaL_setfuncs(lua, ObjectConstructorMetatable::Metatable, 0);

		{
			int meta = lua_gettop(lua);

			lua_pushstring(lua, "__metatable");
			lua_pushstring(lua, "This metatable is protected");

			lua_settable(lua, meta);
		}

		lua_createtable(lua, 0, 0);

		int objectTable = lua_gettop(lua);

		luaL_getmetatable(lua, "objectConstructorMetatable");
		lua_setmetatable(lua, -2);

		const ReflectionData::DataVector& types = ReflectionData::GetTypes();

		for (int i = 0; i < int(types.size()); ++i)
		{
			if (types[i]->Inherits("Object"))
			{
				lua_pushstring(lua, types[i]->Name.c_str());

				LuaData* newData = (LuaData*)lua_newuserdata(lua, sizeof(LuaData));

				new (newData) LuaData(LuaType::Object);

				newData->Meta = types[i];

				luaL_getmetatable(lua, "objectMetatable");
				lua_setmetatable(lua, -2);

				lua_settable(lua, objectTable);
			}
		}

		lua_pushvalue(lua, objectTable);
		lua_setglobal(lua, "GameObject");

		lua_gc(lua, LUA_GCSETPAUSE, 100);
		lua_gc(lua, LUA_GCSETSTEPMUL, 100);

		lua_pushcfunction(lua, typeof);
		lua_setglobal(lua, "typeof");

		{

			luaL_newmetatable(lua, "typeConstructorMetatable");
			luaL_setfuncs(lua, TypeConstructorMetatable::Metatable, 0);

			lua_createtable(lua, 0, 0);

			int typesTable = lua_gettop(lua);

			const ReflectionData::DataVector& reflected = ReflectionData::GetTypes();

			for (int i = 0; i < int(reflected.size()); ++i)
			{
				const ReflectionData* type = reflected[i];

				if (type->Create != nullptr)
				{
					LuaData* newData = (LuaData*)lua_newuserdata(lua, sizeof(LuaData));

					new (newData) LuaData(LuaType::Object);

					newData->Meta = types[i];

					luaL_getmetatable(lua, "typeConstructorMetatable");
					lua_setmetatable(lua, -2);

					lua_pushstring(lua, types[i]->Name.c_str());
					lua_pushvalue(lua, -2);
					lua_settable(lua, typesTable);

					lua_setglobal(lua, types[i]->Name.c_str());
				}
			}

			lua_setglobal(lua, "types");
		}

		MakeLuaReference(lua, Engine::Root()->GetObjectID());

		lua_setglobal(lua, "Engine");

		lua_pushcfunction(lua, LuaGameTime);
		lua_setglobal(lua, "gameTime");

		lua_pushstring(lua, "functions");
		lua_createtable(lua, 10, 0);
		lua_settable(lua, LUA_REGISTRYINDEX);

		lua_pushstring(lua, "threads");
		lua_createtable(lua, 10, 0);
		lua_settable(lua, LUA_REGISTRYINDEX);

		{
			luaL_loadstring(lua,
				"	return function(queueThread)"
				"\n		function wait(time)"
				"\n			time = time or 1/120"
				"\n			if type(time) ~= 'number' then"
				"\n				error('Expected type \"number\" for argument 1 of \"wait\", got \"' .. type(callback) .. '\"\\\n')"
				"\n			end"
				"\n			"
				"\n			local start = queueThread(time)"
				"\n			"
				"\n			coroutine.yield()"
				"\n			"
				"\n			return gameTime() - start"
				"\n		end"
				"\n		"
				"\n		return wait"
				"\n	end"
			);

			lua_call(lua, 0, 1);
			lua_pushcfunction(lua, LuaQueueThread);
			lua_call(lua, 1, 1);
			lua_setglobal(lua, "wait");
		}

		{
			luaL_loadstring(lua,
				"	return function(queueThread, debug)"
				"\n		function delay(time, callback, ...)"
				"\n			if type(callback) ~= 'function' then"
				"\n				error('Expected type \"function\" for argument 2 of \"delay\", got \"' .. type(callback) .. '\"')"
				"\n			elseif type(time) ~= 'number' then"
				"\n				error('Expected type \"number\" for argument 1 of \"delay\", got \"' .. type(time) .. '\"')"
				"\n			end"
				"\n			"
				"\n			--local sourceThread = coroutine.running()"
				"\n			"
				"\n			--local entryTrace = debug.traceback()"
				"\n			"
				"\n			coroutine.wrap(function(...)"
				"\n				wait(time)"
				"\n				"
				"\n				--[[if scriptAlive(sourceThread) then"
				"\n					return"
				"\n				end]]"
				"\n				"
				"\n				local success, result = pcall(callback, ...)"
				"\n				"
				"\n				if not success then"
				"\n					print(result)"
				"\n					--print(entryTrace)"
				"\n				end"
				"\n			end)(...)"
				"\n		end"
				"\n		"
				"\n		return delay"
				"\n	end"
			);

			lua_call(lua, 0, 1);
			lua_pushcfunction(lua, LuaQueueThread);
			lua_getglobal(lua, "debug");
			lua_call(lua, 2, 1);
			lua_setglobal(lua, "delay");
		}

		{
			luaL_loadstring(lua,
				"	return function(queueThread, debug)"
				"\n		function timer(time, callback, ...)"
				"\n			if type(callback) ~= 'function' then"
				"\n				error('Expected type \"function\" for argument 2 of \"timer\", got \"' .. type(callback) .. '\"\\\n')"
				"\n			elseif type(time) ~= 'number' then"
				"\n				error('Expected type \"number\" for argument 1 of \"timer\", got \"' .. type(time) .. '\"')"
				"\n			end"
				"\n			"
				"\n			--local sourceThread = coroutine.running()"
				"\n			"
				"\n			--local entryTrace = debug.traceback()"
				"\n			"
				"\n			coroutine.wrap(function(...)"
				"\n				while true do"
				"\n					wait(time)"
				"\n					"
				"\n					--[[if coroutine.status(sourceThread) == 'dead' then"
				"\n						break"
				"\n					end]]"
				"\n					"
				"\n					local success, result = pcall(callback, ...)"
				"\n					"
				"\n					if not success then"
				"\n						print(result)"
				"\n						--print(entryTrace)"
				"\n					end"
				"\n					"
				"\n					if not success then"
				"\n						break"
				"\n					end"
				"\n				end"
				"\n			end)(...)"
				"\n		end"
				"\n		"
				"\n		return timer"
				"\n	end"
			);

			lua_call(lua, 0, 1);
			lua_pushcfunction(lua, LuaQueueThread);
			lua_getglobal(lua, "debug");
			lua_call(lua, 2, 1);
			lua_setglobal(lua, "timer");

			luaL_newlib(lua, LuaJsonLib);
			lua_setglobal(lua, "json");
		}

		Engine::LuaThread::Initialize(lua);
	}

	LuaFunctionHeap LuaFunctions;
	LuaThreadHeap LuaThreads;

	struct LuaThreadMarker
	{
		int ID = -1;
		lua_State* State = nullptr;

		LuaThreadMarker(lua_State* lua, int id) : ID(id), State(lua) {}
	};

	typedef std::vector<int> LuaThreadQueue;
	typedef IDHeap<lua_State*> LuaThreadHeap;

	LuaThreadQueue resumedThreads;
	LuaThreadHeap luaThreads;

	void UpdateLua(lua_State* lua, float delta)
	{
		lua_gc(lua, LUA_GCCOLLECT, 0);

		for (int i = 0; i < int(resumedThreads.size()); ++i)
		{
			lua_pushstring(lua, "threads");
			lua_gettable(lua, LUA_REGISTRYINDEX);

			int threads = lua_gettop(lua);

			lua_pushnumber(lua, lua_Number(resumedThreads[i]));
			lua_pushnil(lua);
			lua_settable(lua, threads);

			lua_pop(lua, 1);

			lua_State* thread = luaThreads.GetNode(resumedThreads[i]).GetData();

			luaThreads.Release(resumedThreads[i]);

			LuaThread::CoroutineResume(thread);
			lua_resume(thread, lua, 0);
		}

		resumedThreads.clear();

		Engine::LuaThread::Update(lua, delta);
	}

	int LuaQueueThread(lua_State* lua)
	{
		if (lua_isnumber(lua, 1))
		{
			float delay = float(lua_tonumber(lua, 1));
			int id = luaThreads.RequestID(lua);

			lua_pushstring(lua, "threads");
			lua_gettable(lua, LUA_REGISTRYINDEX);

			int threads = lua_gettop(lua);

			lua_pushnumber(lua, lua_Number(id));
			lua_pushthread(lua);
			lua_settable(lua, threads);

			TaskScheduler::Repeat(delay, 1, [id] (float, float, int)
			{
				resumedThreads.push_back(id);
			});
		}
		else
			Lua::BadArgumentError(lua, 1, "number", Lua::GetType(lua, 1), "internal: QueueThread");

		return LuaGameTime(lua);
	}

	int LuaGameTime(lua_State* lua)
	{
		lua_pushnumber(lua, lua_Number(Graphics::ActiveWindow->GetTime()));

		return 1;
	}

	void MakeLuaReference(lua_State* lua, int id)
	{
		std::shared_ptr<Engine::ObjectBase> object = Engine::Object::GetObjectFromID(id);

		LuaData* newData = (LuaData*)lua_newuserdata(lua, sizeof(LuaData));

		int dataIndex = lua_gettop(lua);

		new (newData) LuaData(LuaType::Object);

		newData->Meta = ReflectionData::GetType(object->GetTypeName());
		newData->Data = &*object;
		newData->Reference = object;

		luaL_getmetatable(lua, "objectMetatable");
		lua_setmetatable(lua, -2);

		lua_pushstring(lua, "Objects");
		lua_gettable(lua, LUA_REGISTRYINDEX);

		int tableIndex = lua_gettop(lua);

		lua_pushnumber(lua, lua_Number(object->GetObjectID()));
		lua_pushvalue(lua, dataIndex);
		lua_settable(lua, tableIndex);
		lua_pop(lua, 1);
	}

	void MakeLuaTypeReference(lua_State* lua, const ReflectionData* type, void* data)
	{

		LuaData* newData = (LuaData*)lua_newuserdata(lua, sizeof(LuaData));

		int dataIndex = lua_gettop(lua);

		new (newData) LuaData(LuaType::Type);

		newData->Meta = type;
		newData->Data = data;

		luaL_getmetatable(lua, "objectMetatable");
		lua_setmetatable(lua, -2);
	}
}
