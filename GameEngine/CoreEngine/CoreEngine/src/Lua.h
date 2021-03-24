// https://www.lua.org/manual/5.3/manual.html
#pragma once

#include <lua.hpp>
#include <sstream>
#include <vector>

#include "ReflectionData.h"

typedef const ReflectionData& DataType;
typedef ReflectionData::Member DataMember;

typedef std::function<std::string(void*)> LuaToStringCallback;

typedef int(*LuaFunction)(lua_State*);

typedef ReflectionData::FunctionOverload<int(lua_State*)> LuaMethod;

struct LuaObject
{
	const char* typeName;
	int typeID;
	void* data;
};

class Lua
{
public:
	typedef std::vector<std::string> StringVector;
	typedef std::function<int(lua_State*)> LuaIterator;

	Lua();
	~Lua();

	lua_State* GetState();

	bool DoFile(const char* filePath);

	static void InitializeLuaTypes();

	static void DumpStack(lua_State* lua);
	static int Traceback(lua_State* lua);
	static void ClearBuffer();
	static void SetErrorMessage(const std::string& message);
	static void Error(lua_State* lua);
	static void BadArgumentError(lua_State* lua, int argument, const char* expectedType, const char* luaType, const char* functionName = "");
	static void BadWriteError(lua_State *lua, const char* memberName, const char* parentName, const char* typeName, const char* luaType);
	static void BadIndexError(lua_State* lua, const char* expectedType, int objectIndex, int paramIndex);
	static void BadMemberError(lua_State* lua, int objectIndex, int paramIndex);
	static void BadCallError(lua_State* lua, int index);
	static void BadTypeError(lua_State* lua, int index);
	static void BadLengthError(lua_State* lua, int index);
	static void BadArithmeticError(lua_State* lua, int index);
	static void BadConcatError(lua_State* lua, int index);
	static void BadBitOpError(lua_State* lua, int index);
	static void BadComparisonError(lua_State* lua, int index1, int index2);
	static int ToString(lua_State* lua, int index);

	static bool RunChunk(lua_State* lua, const char* source, const char* fileName, int lineNumber, int length = -1);

	static bool TypeMatches(lua_State* lua, int index, const ReflectionData* type);
	static const char* GetType(lua_State* lua, int index = -1);
	static bool ArgumentsMatch(lua_State* lua, const ReflectionData::ParameterVector& parameters, int startIndex = 0);
	static int GetOverload(lua_State* lua, const ReflectionData::Function* method, int startIndex = 0);

	bool GetBoolean(lua_State* lua, int argument, int index = -1);
	double GetNumber(lua_State* lua, int argument, int index = -1);
	const char* GetString(lua_State* lua, int argument, int index = -1);
	void* GetObject(lua_State* lua, const char* className, int argument, int index = -1);
	int GetObject(lua_State* lua, const char* className, int* object, int argument, int index = -1);
	int GetThread(lua_State* lua, int* thread, int argument, int index = -1);
	int GetFunction(lua_State* lua, int* function, int argument, int index = -1);
	int ReleaseThread(lua_State* lua, int* thread);
	int ReleaseFunction(lua_State* lua, int* function);
	int ReleaseObject(lua_State* lua, int* object);

	static void UnpackTable(lua_State* lua, const LuaIterator& iterator, int argument, int index = -1);
	static void PackTable(lua_State* lua, const LuaIterator& iterator, int arraySize = 0, int mapSize = 0);

	static int CallFunction(lua_State* lua, int function, int arguments, int results);

	static void ToLua(lua_State* lua, const DataMember* member, void* data, int index = -1);
	static void FromLua(lua_State* lua, const DataMember* member, void* data, int index = -1);

private:
	static StringVector StackTraceback;
	static std::stringstream ErrorBuffer;
	static std::string ErrorMessage;

	lua_State* State;

	class LuaCallback : ReflectionData
	{
	public:
		LuaCallback() : ReflectionData("LuaFunction") {}

		void Copy(void* object, void* data) const { *((int*)object) = *((int*)data); }

		std::string Format(void* object) const { return ""; }

		void Parse(void* object, const std::string& data) const {}

		static void Initialize() { new LuaCallback(); }
	};

	class LuaThread : ReflectionData
	{
	public:
		LuaThread() : ReflectionData("LuaThread") {}

		void Copy(void* object, void* data) const { *((int*)object) = *((int*)data); }

		std::string Format(void* object) const { return ""; }

		void Parse(void* object, const std::string& data) const {}

		static void Initialize() { new LuaThread(); }
	};

	class LuaTable : ReflectionData
	{
	public:
		LuaTable() : ReflectionData("LuaTable") {}

		void Copy(void* object, void* data) const { *((int*)object) = *((int*)data); }

		std::string Format(void* object) const { return ""; }

		void Parse(void* object, const std::string& data) const {}

		static void Initialize() { new LuaTable(); }
	};
};
