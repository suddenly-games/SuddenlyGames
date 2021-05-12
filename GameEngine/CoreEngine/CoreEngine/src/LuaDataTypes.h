#pragma once

#ifdef LuaStackTypesValid

#include "Lua.h"

namespace Engine
{
	namespace LuaTypes
	{
		class Lua_bool
		{
		public:
			template <typename T>
			class Converter
			{
			public:
				Converter(int argumentNumber, int& arguments, bool isStatic, const T& defaultValue = T()) : ArgumentNumber(argumentNumber), Arguments(arguments), IsStatic(isStatic), DefaultValue(defaultValue) {}

				lua_State** LuaState = nullptr;
				const char* FuncName = "";
				int ArgumentNumber = 0;
				int& Arguments = ArgumentNumber;
				bool HasDefaultValue = false;
				bool IsStatic = false;
				T DefaultValue = T();

				operator T() const
				{
					int index = ArgumentNumber + (IsStatic ? 1 : 2);

					if (lua_isboolean(*LuaState, index))
						return lua_toboolean(*LuaState, index);
					else if (HasDefaultValue)
						return DefaultValue;
					else
						Lua::BadArgumentError(*LuaState, ArgumentNumber + 1, "bool", Lua::GetType(*LuaState, index));

					return false;
				};
			};

			class ReturnValue
			{
			public:
				const int ReturnValues = 1;

				lua_State** LuaState = nullptr;

				void operator()(bool returnValue)
				{
					lua_pushboolean(*LuaState, returnValue);
				}
			};
		};

		class Lua_nil
		{
		public:
			class ReturnValue
			{
			public:
				const int ReturnValues = 0;

				lua_State** LuaState = nullptr;

				void operator()()
				{
					lua_pushnil(*LuaState);
				}
			};
		};

		class Lua_number
		{
		public:
			template <typename T>
			class Converter
			{
			public:
				Converter(int argumentNumber, int& arguments, bool isStatic, const T& defaultValue = T()) : ArgumentNumber(argumentNumber), Arguments(arguments), IsStatic(isStatic), DefaultValue(defaultValue) {}

				lua_State** LuaState = nullptr;
				const char* FuncName = "";
				int ArgumentNumber = 0;
				int& Arguments = ArgumentNumber;
				bool HasDefaultValue = false;
				bool IsStatic = false;
				T DefaultValue = T();

				operator T() const
				{
					int index = ArgumentNumber + (IsStatic ? 1 : 2);

					if (lua_isnumber(*LuaState, index))
						return (T)(lua_tonumber(*LuaState, index));
					else if (HasDefaultValue)
						return DefaultValue;
					else
						Lua::BadArgumentError(*LuaState, ArgumentNumber + 1, "number", Lua::GetType(*LuaState, index));

					return 0;
				};
			};

			class ReturnValue
			{
			public:
				const int ReturnValues = 1;

				lua_State** LuaState = nullptr;

				void operator()(long long returnValue)
				{
					lua_pushnumber(*LuaState, lua_Number(returnValue));
				}

				void operator()(unsigned long long returnValue)
				{
					lua_pushnumber(*LuaState, lua_Number(returnValue));
				}

				void operator()(long returnValue)
				{
					lua_pushnumber(*LuaState, lua_Number(returnValue));
				}

				void operator()(unsigned long returnValue)
				{
					lua_pushnumber(*LuaState, lua_Number(returnValue));
				}

				void operator()(int returnValue)
				{
					lua_pushnumber(*LuaState, lua_Number(returnValue));
				}

				void operator()(unsigned int returnValue)
				{
					lua_pushnumber(*LuaState, lua_Number(returnValue));
				}

				void operator()(short returnValue)
				{
					lua_pushnumber(*LuaState, lua_Number(returnValue));
				}

				void operator()(unsigned short returnValue)
				{
					lua_pushnumber(*LuaState, lua_Number(returnValue));
				}

				void operator()(char returnValue)
				{
					lua_pushnumber(*LuaState, lua_Number(returnValue));
				}

				void operator()(unsigned char returnValue)
				{
					lua_pushnumber(*LuaState, lua_Number(returnValue));
				}

				void operator()(float returnValue)
				{
					lua_pushnumber(*LuaState, lua_Number(returnValue));
				}

				void operator()(double returnValue)
				{
					lua_pushnumber(*LuaState, lua_Number(returnValue));
				}
			};
		};

		class Lua_string
		{
		public:
			template <typename T>
			class Converter
			{
			public:
				Converter(int argumentNumber, int& arguments, bool isStatic, const T& defaultValue = T()) : ArgumentNumber(argumentNumber), Arguments(arguments), IsStatic(isStatic), DefaultValue(defaultValue) {}

				lua_State** LuaState = nullptr;
				const char* FuncName = "";
				int ArgumentNumber = 0;
				int& Arguments = ArgumentNumber;
				bool HasDefaultValue = false;
				bool IsStatic = false;
				T DefaultValue = T();

				operator T() const
				{
					int index = ArgumentNumber + (IsStatic ? 1 : 2);

					if (lua_type(*LuaState, index) == LUA_TSTRING)
						return lua_tostring(*LuaState, index);
					else if (HasDefaultValue)
						return DefaultValue;
					else
						Lua::BadArgumentError(*LuaState, ArgumentNumber + 1, "string", Lua::GetType(*LuaState, index));

					return "";
				};
			};

			class ReturnValue
			{
			public:
				const int ReturnValues = 1;

				lua_State** LuaState = nullptr;

				void operator()(const std::string& returnValue)
				{
					lua_pushstring(*LuaState, returnValue.c_str());
				}
			};
		};
	}
}
#endif
