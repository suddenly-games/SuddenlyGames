#pragma once

#include "Enum.h"
#include "Lua.h"
#include "CoreTypes.h"

namespace Engine
{
	class ObjectBase;
	class Object;
	class ModelAsset;

	template <typename A, typename B>
	std::shared_ptr<A> CastSmartPointer(const std::shared_ptr<B>& pointer)
	{
		return std::static_pointer_cast<A>(pointer);
	}

	namespace LuaTypes
	{
		class Lua_object
		{
		public:
			static int GetObjectID(const void*);
			static void PushObject(lua_State* lua, const std::shared_ptr<Engine::ObjectBase>& object);

			template <typename T>
			class Converter;

			template <typename T>
			class Converter<std::shared_ptr<T>>
			{
			public:
				Converter(int argumentNumber, int& arguments, bool isStatic, const std::shared_ptr<T>& defaultValue = std::shared_ptr<T>()) : ArgumentNumber(argumentNumber), Arguments(arguments), IsStatic(isStatic), DefaultValue(defaultValue) {}

				lua_State** LuaState = nullptr;
				int ArgumentNumber = 0;
				int& Arguments = ArgumentNumber;
				bool HasDefaultValue = false;
				bool IsStatic = false;
				std::shared_ptr<T> DefaultValue = std::shared_ptr<T>();

				operator std::shared_ptr<T>() const
				{
					int index = ArgumentNumber + (IsStatic ? 1 : 2);

					if (lua_isuserdata(*LuaState, index))
						return ObjectBase::GetObjectFromID(GetObjectID(lua_topointer(*LuaState, index)))->Cast<T>();
					else if (HasDefaultValue && (lua_gettop(*LuaState) < index || !lua_isnil(*LuaState, index)))
						return DefaultValue;
					else
						Lua::BadArgumentError(*LuaState, ArgumentNumber + 1, "Object", Lua::GetType(*LuaState, index));

					return nullptr;
				};
			};

			template <typename T>
			class Converter<std::weak_ptr<T>>
			{
			public:
				Converter(int argumentNumber, int& arguments, bool isStatic, const std::weak_ptr<T>& defaultValue = std::weak_ptr<T>()) : ArgumentNumber(argumentNumber), Arguments(arguments), IsStatic(isStatic), DefaultValue(defaultValue) {}

				lua_State** LuaState = nullptr;
				int ArgumentNumber = 0;
				int& Arguments = ArgumentNumber;
				bool HasDefaultValue = false;
				bool IsStatic = false;
				std::weak_ptr<T> DefaultValue = std::weak_ptr<T>();

				operator std::weak_ptr<T>() const
				{
					int index = ArgumentNumber + (IsStatic ? 1 : 2);

					if (lua_isuserdata(*LuaState, index))
						return ObjectBase::GetObjectFromID(GetObjectID(lua_topointer(*LuaState, index)))->Cast<T>();
					else if (HasDefaultValue && (lua_gettop(*LuaState) < index || !lua_isnil(*LuaState, index)))
						return DefaultValue;
					else
						Lua::BadArgumentError(*LuaState, ArgumentNumber + 1, "Object", Lua::GetType(*LuaState, index));

					return nullptr;
				};
			};

			class ReturnValue
			{
			public:
				const int ReturnValues = 1;

				lua_State** LuaState = nullptr;

				void operator()(const std::shared_ptr<Engine::ObjectBase>& object)
				{
					if (object == nullptr)
						lua_pushnil(*LuaState);
					else
						PushObject(*LuaState, object);
				}

				//void operator()(const std::weak_ptr<Engine::ObjectBase>& object)
				//{
				//	if (object.expired())
				//		lua_pushnil(*LuaState);
				//	else
				//		PushObject(*LuaState, object.lock());
				//}
			};
		};

		template <typename LuaEnumType>
		class Lua_enum
		{
		public:
			static std::string EnumTypeName(const std::string& name = "")
			{
				static bool initialized = false;
				static std::string typeName = "Enum";

				if (initialized)
					return typeName;

				initialized = true;
				typeName = name;

				return typeName;
			}

			template <typename T>
			class Converter
			{
			public:
				Converter(int argumentNumber, int& arguments, bool isStatic, const LuaEnumType& defaultValue = LuaEnumType()) : ArgumentNumber(argumentNumber), Arguments(arguments), IsStatic(isStatic), DefaultValue(defaultValue) {}

				lua_State** LuaState = nullptr;
				int ArgumentNumber = 0;
				int& Arguments = ArgumentNumber;
				bool HasDefaultValue = false;
				bool IsStatic = false;
				LuaEnumType DefaultValue = LuaEnumType();

				operator LuaEnumType() const
				{
					int index = ArgumentNumber + (IsStatic ? 1 : 2);
					std::string name = EnumTypeName();
					const Engine::EnumType& type = Engine::EnumType::GetType(name);

					if (lua_isnumber(*LuaState, index))
						return LuaEnumType(int(lua_tonumber(*LuaState, index)));
					else if (lua_type(*LuaState, index) == LUA_TSTRING)
					{
						const char* item = lua_tostring(*LuaState, index);

						typename Engine::EnumType::EnumItemNameMap::const_iterator i = type.ItemNames.find(item);

						if (i != type.ItemNames.end())
							return LuaEnumType(type.Items.find(i->second)->second.Value);
						else
						{
							Lua::SetErrorMessage("Attempt to use invalid enum item '" + std::string(item) + "' of enum type '" + EnumTypeName() + "'");
							Lua::Error(*LuaState);
						}
					}
					else if (lua_isuserdata(*LuaState, index))
					{
						LuaData* data = (LuaData*)lua_topointer(*LuaState, index);

						if (data == nullptr)
						{
							Lua::SetErrorMessage("Received nil value userdata for " + name);
							Lua::Error(*LuaState);
						}

						if (data->Meta != nullptr && data->Meta->IsEnum && data->Meta->ParentName == name)
							return LuaEnumType(*((int*)data->Meta->GetMember("Value")->GetLocation(nullptr)));
						else
							Lua::BadArgumentError(*LuaState, ArgumentNumber + 1, ("Enum." + name).c_str(), Lua::GetType(*LuaState, index));
					}
					else if (!HasDefaultValue || !(lua_gettop(*LuaState) < index || !lua_isnil(*LuaState, index)))
						Lua::BadArgumentError(*LuaState, ArgumentNumber + 1, ("Enum." + name).c_str(), Lua::GetType(*LuaState, index));

					return DefaultValue;
				};
			};

			class ReturnValue
			{
			public:
				const int ReturnValues = 1;

				lua_State** LuaState = nullptr;

				void operator()(const LuaEnumType& object)
				{
					const Engine::EnumType& type = Engine::EnumType::GetType(EnumTypeName());
				
					typename Engine::EnumType::EnumItemMap::const_iterator i = type.Items.find(int(object));

					if (i == type.Items.end())
					{
						lua_pushnil(*LuaState);

						return;
					}

					lua_getglobal(*LuaState, "Enum");

					int enums = lua_gettop(*LuaState);

					lua_pushstring(*LuaState, EnumTypeName().c_str());
					lua_gettable(*LuaState, enums);

					int enumType = lua_gettop(*LuaState);

					lua_pushstring(*LuaState, i->second.Name.c_str());
					lua_gettable(*LuaState, enumType);
				}
			};
		};
	}

	template <>
	class CoreTypes<std::shared_ptr<Engine::ObjectBase>>
	{
	public:
		typedef LuaTypes::Lua_object LuaType;

		static const std::string GetTypeName() { return "ObjectBase"; };
	};

	template <typename T>
	class CoreTypes<std::shared_ptr<T>>
	{
	public:
		typedef LuaTypes::Lua_object LuaType;

		static const std::string& RegisterTypeName(const std::string& name)
		{
			static std::string typeName = "ObjectBase";
			static bool initialized = false;

			if (initialized)
				return typeName;

			initialized = true;
			typeName = name;

			return typeName;
		}
		static const std::string GetTypeName() { return RegisterTypeName(""); };
	};

	template <typename T>
	class CoreTypes<std::weak_ptr<T>>
	{
	public:
		typedef LuaTypes::Lua_object LuaType;

		static const std::string& RegisterTypeName(const std::string& name)
		{
			return CoreTypes<std::shared_ptr<T>>::RegisterTypeName(name);
		}
		static const std::string GetTypeName() { return CoreTypes<std::shared_ptr<T>>::GetTypeName(); };
	};

	template <typename T>
	class CoreTypes<LuaEnum<T>>
	{
	public:
		typedef LuaTypes::Lua_enum<T> LuaType;

		static const std::string& RegisterTypeName(const std::string& name)
		{
			static std::string typeName = "Enum";
			static bool initialized = false;

			if (initialized)
				return typeName;

			initialized = true;
			typeName = name;
			LuaType::EnumTypeName(typeName);

			return typeName;
		}
		static const std::string GetTypeName() { return RegisterTypeName(""); };
	};
}
