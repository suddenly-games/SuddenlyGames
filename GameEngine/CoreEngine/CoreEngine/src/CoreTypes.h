#pragma once

#ifdef LuaStackTypesValid



namespace Engine
{
	template <typename T>
	class CoreTypes {};

	template <>
	class CoreTypes<bool>
	{
	public:
		typedef LuaTypes::Lua_bool LuaType;

		static const std::string GetTypeName() { return "bool"; };
	};

	template <>
	class CoreTypes<std::string>
	{
	public:
		typedef LuaTypes::Lua_string LuaType;

		static const std::string GetTypeName() { return "string"; };
	};

	template <>
	class CoreTypes<long long>
	{
	public:
		typedef LuaTypes::Lua_number LuaType;

		static const std::string GetTypeName() { return "long long"; };
	};

	template <>
	class CoreTypes<unsigned long long>
	{
	public:
		typedef LuaTypes::Lua_number LuaType;

		static const std::string GetTypeName() { return "unsigned long long"; };
	};

	template <>
	class CoreTypes<long>
	{
	public:
		typedef LuaTypes::Lua_number LuaType;

		static const std::string GetTypeName() { return "long"; };
	};

	template <>
	class CoreTypes<unsigned long>
	{
	public:
		typedef LuaTypes::Lua_number LuaType;

		static const std::string GetTypeName() { return "unsigned long"; };
	};

	template <>
	class CoreTypes<int>
	{
	public:
		typedef LuaTypes::Lua_number LuaType;

		static const std::string GetTypeName() { return "int"; };
	};

	template <>
	class CoreTypes<unsigned int>
	{
	public:
		typedef LuaTypes::Lua_number LuaType;

		static const std::string GetTypeName() { return "unsigned int"; };
	};

	template <>
	class CoreTypes<short>
	{
	public:
		typedef LuaTypes::Lua_number LuaType;

		static const std::string GetTypeName() { return "short"; };
	};

	template <>
	class CoreTypes<unsigned short>
	{
	public:
		typedef LuaTypes::Lua_number LuaType;

		static const std::string GetTypeName() { return "unsigned short"; };
	};

	template <>
	class CoreTypes<char>
	{
	public:
		typedef LuaTypes::Lua_number LuaType;

		static const std::string GetTypeName() { return "char"; };
	};

	template <>
	class CoreTypes<unsigned char>
	{
	public:
		typedef LuaTypes::Lua_number LuaType;

		static const std::string GetTypeName() { return "unsigned char"; };
	};

	template <>
	class CoreTypes<float>
	{
	public:
		typedef LuaTypes::Lua_number LuaType;

		static const std::string GetTypeName() { return "float"; };
	};

	template <>
	class CoreTypes<double>
	{
	public:
		typedef LuaTypes::Lua_number LuaType;

		static const std::string GetTypeName() { return "double"; };
	};
}
#endif
