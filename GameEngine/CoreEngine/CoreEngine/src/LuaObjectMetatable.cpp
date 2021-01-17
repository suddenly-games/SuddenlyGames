#include "LuaObjectMetatable.h"

#include <iostream>

#include "LuaBinding.h"
#include "ReflectionData.h"
#include "ObjectReflection.h"
#include "Object.h"

namespace Engine
{
	const luaL_Reg ObjectMetatable::Metatable[25] = {
		{ "__index",	__index },
		{ "__newindex",	__newindex },
		{ "__tostring",	__tostring },
		{ "__call",		__call },
		{ "__len",		__len },
		{ "__gc",		__gc },
		{ "__unm",		__unaryMinus },
		{ "__add",		__add },
		{ "__sub",		__subtract },
		{ "__mul",		__multiply },
		{ "__div",		__divide },
		{ "__idiv",		__floorDivide },
		{ "__mod",		__modulus },
		{ "__pow",		__power },
		{ "__concat",	__concatenate },
		{ "__band",		__bitwiseAnd },
		{ "__bor",		__bitwiseOr },
		{ "__bxor",		__bitwiseXOr },
		{ "__bnot",		__bitwiseNot },
		{ "__bshl",		__bitwiseShiftLeft },
		{ "__bshr",		__bitwiseShiftRight },
		{ "__eq",		__equals },
		{ "__lt",		__lessThan },
		{ "__le",		__lessThanEqualTo },
		{0, 0}
	};

	LuaData* checkLuaData(lua_State* lua, const void* data)
	{
		LuaData* object = reinterpret_cast<LuaData*>(const_cast<void*>(data));

		//if (object == nullptr || object->Data == nullptr)
		//	Lua::BadArgumentError(lua, 0, "Object", "nil");

		return object;
	}

	int ObjectMetatable::__index(lua_State* lua)
	{
		LuaData* object = (LuaData*)lua_topointer(lua, 1);

		if (lua_type(lua, 2) == LUA_TSTRING)
		{
			const char* index = lua_tostring(lua, 2);

			const FunctionOverloads* method = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction(index));

			if (method != nullptr)
			{
				lua_pushcfunction(lua, method->Callback);

				return 1;
			}

			const ReflectionData::Member* member = object->Meta->GetMember(index);

			if (member != nullptr)
			{
				if (object->Data == nullptr && !member->IsStatic)
				{
					Lua::SetErrorMessage("Attempt to use index of non-static member '" + member->Name + "' from type '" + member->ParentName + "' on static type constructor '" + object->Meta->Name.c_str() + "'");
					Lua::Error(lua);

					return 0;
				}

				Lua::ToLua(lua, member, object->Data, -1);

				return 1;
			}
			else if (object->Type == LuaData::LuaType::Object || object->Type == LuaData::LuaType::Type)
			{
				const ClassData::Property* property = nullptr;
				
				if (object->Data != nullptr)
					property = reinterpret_cast<const ClassData*>(object->Meta)->GetProperty(index);

				if (property != nullptr)
				{
					if (property->Getter == nullptr)
					{
						Lua::SetErrorMessage("Attempt to access property '" + property->Name + "' from type '" + property->ParentType->Name + "' with no getter function.");
						Lua::Error(lua);

						return 0;
					}

					return property->Getter->Callback(lua);
				}
				else if (object->Type == LuaData::LuaType::Object)
				{
					std::shared_ptr<Object> thisObject = object->Reference->Cast<Object>(); // note: Matrix3.Scale() fails

					std::shared_ptr<Object> child = thisObject->GetByName(index)->Cast<Object>();

					if (child == nullptr)
						Lua::BadMemberError(lua, 1, 2);
					else
					{
						lua_pushstring(lua, "Objects");

						lua_gettable(lua, LUA_REGISTRYINDEX);

						int objectTable = lua_gettop(lua);

						lua_pushnumber(lua, lua_Number(child->GetObjectID()));

						lua_gettable(lua, objectTable);

						if (lua_gettop(lua) == objectTable || lua_isnil(lua, objectTable + 1))
							Engine::MakeLuaReference(lua, child->GetObjectID());
					}

					return 1;
				}
				else
					Lua::BadMemberError(lua, 1, 2);
			}
			else
				Lua::BadMemberError(lua, 1, 2);
		}
		else
			Lua::BadIndexError(lua, "string", 1, 2);

		return 0;
	}

	int ObjectMetatable::__newindex(lua_State* lua)
	{
		LuaData* object = (LuaData*)lua_topointer(lua, 1);

		if (lua_type(lua, 2) == LUA_TSTRING)
		{
			const char* index = lua_tostring(lua, 2);

			const ReflectionData::Member* member = object->Meta->GetMember(index);

			if (member != nullptr)
			{
				if (object->Data == nullptr && !member->IsStatic)
				{
					Lua::SetErrorMessage("Attempt to use index of non-static member '" + member->Name + "' from type '" + member->ParentName + "' on static type constructor '" + object->Meta->Name.c_str() + "'");
					Lua::Error(lua);

					return 0;
				}

				try
				{
					Lua::FromLua(lua, member, object->Data, 3);
				}
				catch (std::string& err)
				{
					Lua::SetErrorMessage(err);
					Lua::Error(lua);
				}

				return 1;
			}
			else
			{
				const ClassData::Property* property = nullptr;

				if (object->Data != nullptr)
					property = reinterpret_cast<const ClassData*>(object->Meta)->GetProperty(index);

				if (property != nullptr)
				{
					for (int i = 0; i < int(property->Setters.size()); ++i)
					{
						if (Lua::TypeMatches(lua, 3, property->Setters[i]->Parameters[0].Type))
						{
							lua_replace(lua, 2);

							return property->Setters[i]->Callback(lua);
						}
					}

					Lua::SetErrorMessage("Attempt to set property '" + property->Name + "' from type '" + property->ParentType->Name + "' with value of type '" + Lua::GetType(lua, 3) + "' with no matching setter.");
					Lua::Error(lua);

					return 0;
				}
				else
					Lua::BadMemberError(lua, 1, 2);
			}
		}
		else
			Lua::BadIndexError(lua, "string", 1, 2);

		return 0;
	}

	int ObjectMetatable::__tostring(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object->Data == nullptr)
		{
			lua_pushstring(lua, object->Meta->Name.c_str());

			return 1;
		}

		const ReflectionData::Function* toString = object->Meta->GetFunction("operator string");

		try
		{
			if (toString != nullptr && toString->Overloads.size() > 0)
				for (int i = 0; i < int(toString->Overloads.size()); ++i)
					if (toString->Overloads[i]->Parameters.size() == 0)
						return toString->Overloads[i]->Cast<ReflectionData::FunctionOverload<int, lua_State*>>()->Callback(lua);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		lua_pushstring(lua, "userdata");

		return 1;
	}

	int ObjectMetatable::__call(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object->Data == nullptr)
		{
			Lua::SetErrorMessage("Attempt to use operator on static type constructor '" + object->Meta->Name + "'");
			Lua::Error(lua);

			return 0;
		}

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("operator()"));

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		Lua::SetErrorMessage("Attempt to call object of type '" + object->Meta->Name + "'");
		Lua::Error(lua);

		return 0;
	}

	int ObjectMetatable::__len(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object->Data == nullptr)
		{
			Lua::SetErrorMessage("Attempt to use operator on static type constructor '" + object->Meta->Name + "'");
			Lua::Error(lua);

			return 0;
		}

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("GetSize"));

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		Lua::SetErrorMessage("Attempt to use operator # on type '" + object->Meta->Name + "'");
		Lua::Error(lua);

		return 0;
	}

	int ObjectMetatable::__gc(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("LuaGCed"));

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
		}
		catch (std::string& err)
		{
			std::cout << err << std::endl;
		}

		if (object->Type == LuaData::LuaType::Type && object->Meta->Delete)
		{
			try
			{
				object->Meta->Delete(object->Data);
			}
			catch (std::string & err)
			{
				std::cout << err << std::endl;
			}
		}

		try
		{
			object->~LuaData();
		}
		catch (std::string& err)
		{
			std::cout << err << std::endl;
		}

		lua_pushnil(lua);
		lua_setmetatable(lua, 1);

		return 0;
	}

	int ObjectMetatable::__unaryMinus(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object->Data == nullptr)
		{
			Lua::SetErrorMessage("Attempt to use operator on static type constructor '" + object->Meta->Name + "'");
			Lua::Error(lua);

			return 0;
		}

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("operator-"));

		if (lua_gettop(lua) == 2)
			lua_pop(lua, 1);

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		Lua::SetErrorMessage("Attempt to use operator - on type '" + object->Meta->Name + "'");
		Lua::Error(lua);

		return 0;
	}

	int ObjectMetatable::__add(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object->Data == nullptr)
		{
			Lua::SetErrorMessage("Attempt to use operator on static type constructor '" + object->Meta->Name + "'");
			Lua::Error(lua);

			return 0;
		}

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("operator+"));

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		Lua::SetErrorMessage("Attempt to use operator + on type '" + object->Meta->Name + "'");
		Lua::Error(lua);

		return 0;
	}

	int ObjectMetatable::__subtract(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object->Data == nullptr)
		{
			Lua::SetErrorMessage("Attempt to use operator on static type constructor '" + object->Meta->Name + "'");
			Lua::Error(lua);

			return 0;
		}

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("operator-"));

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		Lua::SetErrorMessage("Attempt to use operator - on type '" + object->Meta->Name + "'");
		Lua::Error(lua);

		return 0;
	}

	int ObjectMetatable::__multiply(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object == nullptr)
			object = checkLuaData(lua, lua_topointer(lua, 2));

		if (object->Data == nullptr)
		{
			Lua::SetErrorMessage("Attempt to use operator on static type constructor '" + object->Meta->Name + "'");
			Lua::Error(lua);

			return 0;
		}

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("operator*"));

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		Lua::SetErrorMessage("Attempt to use operator * on type '" + object->Meta->Name + "'");
		Lua::Error(lua);

		return 0;
	}

	int ObjectMetatable::__divide(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object->Data == nullptr)
		{
			Lua::SetErrorMessage("Attempt to use operator on static type constructor '" + object->Meta->Name + "'");
			Lua::Error(lua);

			return 0;
		}

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("operator/"));

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		Lua::SetErrorMessage("Attempt to use operator / on type '" + object->Meta->Name + "'");
		Lua::Error(lua);

		return 0;
	}

	int ObjectMetatable::__floorDivide(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object->Data == nullptr)
		{
			Lua::SetErrorMessage("Attempt to use operator on static type constructor '" + object->Meta->Name + "'");
			Lua::Error(lua);

			return 0;
		}

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("FloorDivide"));

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		Lua::SetErrorMessage("Attempt to use operator // on type '" + object->Meta->Name + "'");
		Lua::Error(lua);

		return 0;
	}

	int ObjectMetatable::__modulus(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object->Data == nullptr)
		{
			Lua::SetErrorMessage("Attempt to use operator on static type constructor '" + object->Meta->Name + "'");
			Lua::Error(lua);

			return 0;
		}

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("operator%"));

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		Lua::SetErrorMessage("Attempt to use operator % on type '" + object->Meta->Name + "'");
		Lua::Error(lua);

		return 0;
	}

	int ObjectMetatable::__power(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object->Data == nullptr)
		{
			Lua::SetErrorMessage("Attempt to use operator on static type constructor '" + object->Meta->Name + "'");
			Lua::Error(lua);

			return 0;
		}

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("Pow"));

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		Lua::SetErrorMessage("Attempt to use operator ^ on type '" + object->Meta->Name + "'");
		Lua::Error(lua);

		return 0;
	}

	int ObjectMetatable::__concatenate(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object->Data == nullptr)
		{
			Lua::SetErrorMessage("Attempt to use operator on static type constructor '" + object->Meta->Name + "'");
			Lua::Error(lua);

			return 0;
		}

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("Concat"));

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		Lua::SetErrorMessage("Attempt to use operator .. on type '" + object->Meta->Name + "'");
		Lua::Error(lua);

		return 0;
	}

	int ObjectMetatable::__bitwiseAnd(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object->Data == nullptr)
		{
			Lua::SetErrorMessage("Attempt to use operator on static type constructor '" + object->Meta->Name + "'");
			Lua::Error(lua);

			return 0;
		}

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("operator&"));

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		Lua::SetErrorMessage("Attempt to use operator & on type '" + object->Meta->Name + "'");
		Lua::Error(lua);

		return 0;
	}

	int ObjectMetatable::__bitwiseOr(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object->Data == nullptr)
		{
			Lua::SetErrorMessage("Attempt to use operator on static type constructor '" + object->Meta->Name + "'");
			Lua::Error(lua);

			return 0;
		}

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("operator|"));

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		Lua::SetErrorMessage("Attempt to use operator | on type '" + object->Meta->Name + "'");
		Lua::Error(lua);

		return 0;
	}

	int ObjectMetatable::__bitwiseXOr(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object->Data == nullptr)
		{
			Lua::SetErrorMessage("Attempt to use operator on static type constructor '" + object->Meta->Name + "'");
			Lua::Error(lua);

			return 0;
		}

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("operator^"));

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		Lua::SetErrorMessage("Attempt to use operator ~ on type '" + object->Meta->Name + "'");
		Lua::Error(lua);

		return 0;
	}

	int ObjectMetatable::__bitwiseNot(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object->Data == nullptr)
		{
			Lua::SetErrorMessage("Attempt to use operator on static type constructor '" + object->Meta->Name + "'");
			Lua::Error(lua);

			return 0;
		}

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("operator~"));

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		Lua::SetErrorMessage("Attempt to use operator ~ on type '" + object->Meta->Name + "'");
		Lua::Error(lua);

		return 0;
	}

	int ObjectMetatable::__bitwiseShiftLeft(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object->Data == nullptr)
		{
			Lua::SetErrorMessage("Attempt to use operator on static type constructor '" + object->Meta->Name + "'");
			Lua::Error(lua);

			return 0;
		}

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("operator<<"));

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		Lua::SetErrorMessage("Attempt to use operator << on type '" + object->Meta->Name + "'");
		Lua::Error(lua);

		return 0;
	}

	int ObjectMetatable::__bitwiseShiftRight(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object->Data == nullptr)
		{
			Lua::SetErrorMessage("Attempt to use operator on static type constructor '" + object->Meta->Name + "'");
			Lua::Error(lua);

			return 0;
		}

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("operator>>"));

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		Lua::SetErrorMessage("Attempt to use operator >> on type '" + object->Meta->Name + "'");
		Lua::Error(lua);

		return 0;
	}

	int ObjectMetatable::__equals(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object->Data == nullptr)
		{
			lua_pushboolean(lua, lua_rawequal(lua, 1, 2));

			return 1;
		}

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("operator=="));

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
			else
			{
				lua_pushboolean(lua, lua_rawequal(lua, 1, 2));

				return 1;
			}
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		return 0;
	}

	int ObjectMetatable::__lessThan(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object->Data == nullptr)
		{
			Lua::SetErrorMessage("Attempt to use operator on static type constructor '" + object->Meta->Name + "'");
			Lua::Error(lua);

			return 0;
		}

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("operator<"));

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		Lua::SetErrorMessage("Attempt to use operator < on type '" + object->Meta->Name + "'");
		Lua::Error(lua);

		return 0;
	}

	int ObjectMetatable::__lessThanEqualTo(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		if (object->Data == nullptr)
		{
			Lua::SetErrorMessage("Attempt to use operator on static type constructor '" + object->Meta->Name + "'");
			Lua::Error(lua);

			return 0;
		}

		const FunctionOverloads* overload = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction("operator<="));

		try
		{
			if (overload != nullptr)
				return overload->Callback(lua);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		Lua::SetErrorMessage("Attempt to use operator <= on type '" + object->Meta->Name + "'");
		Lua::Error(lua);

		return 0;
	}

	const luaL_Reg ObjectConstructorMetatable::Metatable[3] = {
		{ "__tostring",	__tostring },
		{ "__call",		__call },
		{0, 0}
	};

	int ObjectConstructorMetatable::__tostring(lua_State* lua)
	{
		lua_pushstring(lua, "Object");

		return 1;
	}

	int ObjectConstructorMetatable::__call(lua_State* lua)
	{
		std::string typeName;

		if (lua_type(lua, 2) == LUA_TSTRING)
			typeName = lua_tostring(lua, 2);
		else if (lua_type(lua, 2) == LUA_TUSERDATA)
		{
			LuaData* data = (LuaData*)lua_topointer(lua, 2);

			typeName = data->Meta->Name.substr(0, data->Meta->Name.size() - 4);
		}
		else
			Lua::BadArgumentError(lua, 1, "Enum.ObjectType", Lua::GetType(lua, 2));

		Engine::ObjectBase::FactoryCallback factory = nullptr;

		try
		{
			factory = ObjectBase::GetFactoryFunction(typeName);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		if (factory == nullptr)
			Lua::BadTypeError(lua, 2);

		std::shared_ptr<Object> newObject;

		try
		{
			newObject = factory()->Cast<Object>();

			MakeLuaReference(lua, newObject->GetObjectID());
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		return 1;
	}

	const luaL_Reg TypeConstructorMetatable::Metatable[6] = {
		{ "__tostring",	__tostring },
		{ "__call",		__call },
		{ "__index",	ObjectMetatable::__index },
		{ "__newindex",	ObjectMetatable::__newindex },
		{ "__gc",		ObjectMetatable::__gc },
		{0, 0}
	};

	int TypeConstructorMetatable::__tostring(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		lua_pushstring(lua, object->Meta->Name.c_str());

		return 1;
	}

	int TypeConstructorMetatable::__call(lua_State* lua)
	{
		LuaData* object = checkLuaData(lua, lua_topointer(lua, 1));

		const FunctionOverloads* constructor = dynamic_cast<const FunctionOverloads*>(object->Meta->GetFunction(object->Meta->Name));

		if (constructor == nullptr || object->Meta->CreateRaw == nullptr)
		{
			Lua::SetErrorMessage("Attempt to construct type '" + object->Meta->Name + "' with unregistered constructor");
			Lua::Error(lua);

			return 0;
		}

		try
		{
			//Handle<CoreClass> handle;
			//
			//object->Meta->Create(handle);

			MakeLuaTypeReference(lua, object->Meta, object->Meta->CreateRaw());

			lua_replace(lua, 1);

			constructor->Callback(lua);

			lua_pushvalue(lua, 1);
		}
		catch (std::string& err)
		{
			Lua::SetErrorMessage(err);
			Lua::Error(lua);
		}

		return 1;
	}
}
