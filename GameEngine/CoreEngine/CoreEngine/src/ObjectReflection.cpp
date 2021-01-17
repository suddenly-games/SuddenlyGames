#include "ObjectReflection.h"

#include "LuaBinding.h"
#include "Lua.h"

template<>
std::string fetchInherited<ClassData>()
{
	return "";
}

void* FunctionBinding::GetData(void* data)
{
	Engine::LuaData* object = (Engine::LuaData*)(data);

	return object->Data;
}

int FunctionOverloads::GetOverload(lua_State* lua) const
{
	int largestMatch = -1;
	int leastEnumsMatched = 0x7FFFFFFF;
	int matchingOverload = -1;
	bool isMethod = false;

	if (lua_isuserdata(lua, 1))
	{
		Engine::LuaData* data = (Engine::LuaData*)(lua_topointer(lua, 1));

		isMethod = data->Data != nullptr && data->Meta->Inherits(ParentName);
	}

	for (int i = 0; i < GetOverloads(); ++i)
	{
		ReflectionData::FunctionOverloadData* overload = Overloads[i];

		if (!isMethod && !overload->IsStatic)
			continue;

		if (overload->Parameters.size() == 0)
		{
			if (largestMatch == -1)
			{
				largestMatch = 0;
				matchingOverload = i;
			}
		}
		else
		{
			int start = overload->IsStatic ? 1 : 2;
			int end = isMethod ? 2 : 1;

			for (int startIndex = start; startIndex <= end; ++startIndex)
			{
				bool failed = false;
				int matched = 0;
				int enumsMatched = 0;
				int top = lua_gettop(lua);

				for (int j = 0; j < overload->GetParameters(); ++j)
				{
					int index = j + startIndex;
					ReflectionData::FunctionOverloadData::ParameterData& parameter = overload->Parameters[j];
					bool enumMatch = false;

					if (parameter.Type->IsEnum)
					{
						int luaType = lua_type(lua, index);

						if (luaType == LUA_TNUMBER || luaType == LUA_TSTRING)
						{
							enumMatch = true;
							++enumsMatched;
						}
					}

					if (parameter.HasDefaultValue && (index > top || lua_isnil(lua, index)))
						continue;
					else if (index > top || (!Lua::TypeMatches(lua, index, parameter.Type) && !enumMatch))
					{
						failed = true;

						break;
					}

					matched = j + 1;
				}

				if (!failed && (matched > largestMatch || (matched == largestMatch && enumsMatched < leastEnumsMatched)))
				{
					largestMatch = matched;
					matchingOverload = i;
					leastEnumsMatched = enumsMatched;
				}
			}
		}
	}

	return matchingOverload;
}
