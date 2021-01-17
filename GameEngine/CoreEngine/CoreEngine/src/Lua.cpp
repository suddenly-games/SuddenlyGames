#include "Lua.h"

#include <iostream>

#include "Object.h"
#include "LuaBinding.h"

Lua::StringVector Lua::StackTraceback;
std::stringstream Lua::ErrorBuffer;
std::string Lua::ErrorMessage = std::string();

struct CoreTypeStruct
{
	int Void = -1;
	int Boolean = -1;
	int Character = -1;
	int UnsignedCharacter = -1;
	int Short = -1;
	int UnsignedShort = -1;
	int Integer = -1;
	int UnsignedInteger = -1;
	int Long = -1;
	int UnsignedLong = -1;
	int LongLong = -1;
	int UnsignedLongLong = -1;
	int Float = -1;
	int Double = -1;
	int String = -1;
	int LuaFunction = -1;
	int LuaThread = -1;
	int LuaTable = -1;
};

CoreTypeStruct coreTypeIDs;

Lua::Lua()
{
	State = luaL_newstate();

	luaL_requiref(State, "_G", luaopen_base, 1);
	luaL_requiref(State, LUA_LOADLIBNAME, luaopen_package, 1);
	luaL_requiref(State, LUA_COLIBNAME, luaopen_coroutine, 1);
	luaL_requiref(State, LUA_TABLIBNAME, luaopen_table, 1);
	luaL_requiref(State, LUA_IOLIBNAME, luaopen_io, 1);
	luaL_requiref(State, LUA_OSLIBNAME, luaopen_os, 1);
	luaL_requiref(State, LUA_STRLIBNAME, luaopen_string, 1);
	luaL_requiref(State, LUA_MATHLIBNAME, luaopen_math, 1);
	luaL_requiref(State, LUA_UTF8LIBNAME, luaopen_utf8, 1);

#if defined(LUA_COMPAT_BITLIB)
	luaL_requiref(lua, LUA_BITLIBNAME, luaopen_bit32, 1);
#endif

	lua_pop(State, 9);

	coreTypeIDs.Void = ReflectionData::GetTypeID("void");
	coreTypeIDs.Boolean = ReflectionData::GetTypeID("bool");
	coreTypeIDs.Character = ReflectionData::GetTypeID("char");
	coreTypeIDs.UnsignedCharacter = ReflectionData::GetTypeID("unsigned char");
	coreTypeIDs.Short = ReflectionData::GetTypeID("short");
	coreTypeIDs.UnsignedShort = ReflectionData::GetTypeID("unsigned short");
	coreTypeIDs.Integer = ReflectionData::GetTypeID("int");
	coreTypeIDs.UnsignedInteger = ReflectionData::GetTypeID("unsigned int");
	coreTypeIDs.Long = ReflectionData::GetTypeID("long");
	coreTypeIDs.UnsignedLong = ReflectionData::GetTypeID("unsigned long");
	coreTypeIDs.LongLong = ReflectionData::GetTypeID("long long");
	coreTypeIDs.UnsignedLongLong = ReflectionData::GetTypeID("unsigned long long");
	coreTypeIDs.Float = ReflectionData::GetTypeID("float");
	coreTypeIDs.Double = ReflectionData::GetTypeID("double");
	coreTypeIDs.String = ReflectionData::GetTypeID("string");
	coreTypeIDs.LuaFunction = ReflectionData::GetTypeID("LuaFunction");
	coreTypeIDs.LuaThread = ReflectionData::GetTypeID("LuaThread");
	coreTypeIDs.LuaTable = ReflectionData::GetTypeID("LuaTable");
}

Lua::~Lua()
{
	lua_close(State);
}

lua_State* Lua::GetState()
{
	return State;
}

bool Lua::DoFile(const char* filePath)
{
	lua_pushcfunction(State, Lua::Traceback);

	bool errored = luaL_loadfile(State, filePath);

	if (!errored)
		errored = lua_pcall(State, 0, LUA_MULTRET, -2);

	if (errored)
		std::cout << lua_tostring(State, -1) << std::endl;

	return errored;
}

void Lua::InitializeLuaTypes()
{
	LuaCallback::Initialize();
	LuaThread::Initialize();
	LuaTable::Initialize();
}

void Lua::DumpStack(lua_State* lua)
{
	std::cout << "Lua Stack:" << std::endl;

	int top = lua_gettop(lua);

	for (int i = 1; i <= top; ++i)
		std::cout << "\t[" << i << "]: " << GetType(lua, i) << std::endl;
}

int Lua::Traceback(lua_State* lua)
{
	StackTraceback.clear();

	luaL_traceback(lua, lua, 0, 1);

	const char* trace = lua_tostring(lua, -1);

	for (int i = 0; trace[i];)
	{
		while (trace[i] == ' ' || trace[i] == '\t' || trace[i] == '\n')
			++i;

		int j;

		for (j = 0; trace[i + j] && trace[i + j] != '\n'; ++j);

		std::string line(trace + i, j);

		if (line.size() > 0)
			StackTraceback.push_back(line);

		i += j;
	}

	int start = -1;

	for (int i = 1; start == -1 && i < int(StackTraceback.size()); ++i)
	{
		std::string& line = StackTraceback[i];

		if (!(line[0] == '[' && line[1] == 'C' && line[2] == ']'))
			start = i;
	}

	if (start == -1)
		start = 1;

	std::string& line = StackTraceback[start];

	int size = 0;

	for (int i = 0; line[i] != ' '; ++i)
		if (line[i] == ':')
			size = i + 2;

	const char* message = lua_tostring(lua, 1);

	bool errorThrownByCpp = false;

	for (int i = 0; i < size - 1; ++i)
	{
		if (message[i] != line[i])
		{
			errorThrownByCpp = true;

			break;
		}
	}

	if (errorThrownByCpp)
		ErrorMessage = std::string(line.c_str(), size);
	else
		ErrorMessage = std::string();

	ErrorMessage += message;
	ErrorMessage += "\n\nStack Begin\n";

	for (int i = start; i < int(StackTraceback.size()); ++i)
		ErrorMessage += '\t' + StackTraceback[i] + '\n';

	ErrorMessage += "Stack End";

	lua_pushstring(lua, ErrorMessage.c_str());

	return 1;
}

void Lua::ClearBuffer()
{
	ErrorBuffer.str(std::string());
}

void Lua::Error(lua_State* lua)
{
	ErrorMessage = ErrorBuffer.str();

	lua_pushstring(lua, ErrorMessage.c_str());
	lua_error(lua);

	ClearBuffer();
}

void Lua::SetErrorMessage(const std::string& message)
{
	ErrorBuffer << message;
}

void Lua::BadArgumentError(lua_State* lua, int argument, const char* expectedType, const char* luaType, const char* functionName)
{
	ErrorBuffer << "bad argument to #" << argument << " in function '" << functionName << "': expected '" << expectedType << "', got '" << luaType << "'";
	Error(lua);
}

void Lua::BadWriteError(lua_State *lua, const char* memberName, const char* parentName, const char* typeName, const char* luaType)
{
	ErrorBuffer << "attempt to write bad value to member '" << memberName << "' of '" << parentName << "': expected '" << typeName << "', got '" << luaType << "'";
	Error(lua);
}

void Lua::BadIndexError(lua_State* lua, const char* expectedType, int objectIndex, int paramIndex)
{

	ErrorBuffer << "attempt to index object '" << lua_tostring(lua, ToString(lua, objectIndex)) << "' with type '" << GetType(lua, paramIndex) << "', expected '" << expectedType << "'";
	Error(lua);
}

void Lua::BadMemberError(lua_State* lua, int objectIndex, int paramIndex)
{
	ErrorBuffer << "'" << lua_tostring(lua, ToString(lua, paramIndex)) << "' is not a valid member of '" << luaL_tolstring(lua, ToString(lua, objectIndex), 0) << "' with type '" << GetType(lua, objectIndex) << "'";
	Error(lua);
}

void Lua::BadCallError(lua_State* lua, int index)
{
	ErrorBuffer << "attempt to call '" << luaL_tolstring(lua, ToString(lua, index), 0) << "', a '" << GetType(lua, index) << "' value";
	Error(lua);
}

void Lua::BadTypeError(lua_State* lua, int index)
{
	ErrorBuffer << "cannot create object of type '" << luaL_tolstring(lua, ToString(lua, index), 0) << "'";
	Error(lua);
}

void Lua::BadLengthError(lua_State* lua, int index)
{
	ErrorBuffer << "attempt to get length of type '" << GetType(lua, index) << "'";
	Error(lua);
}

void Lua::BadArithmeticError(lua_State* lua, int index)
{
	ErrorBuffer << "attempt to perform arithmetic on value with type '" << GetType(lua, index) << "'";
	Error(lua);
}

void Lua::BadConcatError(lua_State* lua, int index)
{
	ErrorBuffer << "attempt to concatenate value with type '" << GetType(lua, index) << "'";
	Error(lua);
}

void Lua::BadBitOpError(lua_State* lua, int index)
{
	ErrorBuffer << "attempt to perform bitwise operation on value with type '" << GetType(lua, index) << "'";
	Error(lua);
}

void Lua::BadComparisonError(lua_State* lua, int index1, int index2)
{
	ErrorBuffer << "attempt to compare values of types '" << GetType(lua, index1) << "' and '" << GetType(lua, index2) << "'";
	Error(lua);
}

int Lua::ToString(lua_State* lua, int index)
{
	lua_getglobal(lua, "tostring");
	lua_pushvalue(lua, index);
	lua_call(lua, 1, 1);

	return lua_gettop(lua);
}

bool Lua::TypeMatches(lua_State* lua, int index, const ReflectionData* type)
{
	Engine::LuaData* data = nullptr;

	switch (lua_type(lua, index))
	{
	case LUA_TNIL:
		return type->ID == coreTypeIDs.Void;
	case LUA_TNUMBER:
		return (
			type->ID == coreTypeIDs.Character ||
			type->ID == coreTypeIDs.UnsignedCharacter ||
			type->ID == coreTypeIDs.Short ||
			type->ID == coreTypeIDs.UnsignedShort ||
			type->ID == coreTypeIDs.Integer ||
			type->ID == coreTypeIDs.UnsignedInteger ||
			type->ID == coreTypeIDs.Long ||
			type->ID == coreTypeIDs.UnsignedLong ||
			type->ID == coreTypeIDs.LongLong ||
			type->ID == coreTypeIDs.UnsignedLongLong ||
			type->ID == coreTypeIDs.Float ||
			type->ID == coreTypeIDs.Double
		);
	case LUA_TBOOLEAN:
		return type->ID == coreTypeIDs.Boolean;
	case LUA_TSTRING:
		return type->ID == coreTypeIDs.String;
	case LUA_TTABLE:
		return false;
	case LUA_TFUNCTION:
		return false;
	case LUA_TUSERDATA:
		data = (Engine::LuaData*)(lua_topointer(lua, index));

		return data->Meta->Inherits(type->Name);
	case LUA_TTHREAD:
		return false;
	case LUA_TLIGHTUSERDATA:
		return false;
	default: return false;
	}
}

const char* Lua::GetType(lua_State* lua, int index)
{
	if (lua_isuserdata(lua, index))
	{
		Engine::LuaData* object = (Engine::LuaData*)lua_topointer(lua, index);

		return object->Meta->Name.c_str();
	}
	else
	{
		int type = lua_type(lua, index);

		switch (type)
		{
		case LUA_TNIL: return "nil";
		case LUA_TNUMBER: return "number";
		case LUA_TBOOLEAN: return "bool";
		case LUA_TSTRING: return "string";
		case LUA_TTABLE: return "table";
		case LUA_TFUNCTION: return "function";
		case LUA_TUSERDATA: return "userdata";
		case LUA_TTHREAD: return "thread";
		case LUA_TLIGHTUSERDATA: return "lightuserdata";
		default: return "unknown";
		}
	}
}

bool Lua::ArgumentsMatch(lua_State* lua, const ReflectionData::ParameterVector& parameters, int startIndex)
{
	for (int i = 0; i < int(parameters.size()); ++i)
	{
		bool matched = false;


		if (!matched)
			return false;
	}

	return true;
}

int Lua::GetOverload(lua_State* lua, const ReflectionData::Function* method, int startIndex)
{
	const ReflectionData::OverloadVector& overloads = method->Overloads;

	int largestMatch = -1;
	int matchIndex = -1;

	for (int i = 0; i < int(overloads.size()); ++i)
	{
		bool matched = ArgumentsMatch(lua, overloads[i]->Parameters, startIndex);
		int size = int(overloads[i]->Parameters.size());
		bool largerOverload = size > largestMatch;

		if (matched && largerOverload)
		{
			largestMatch = size;
			matchIndex = i;
		}
	}

	return matchIndex;
}

bool Lua::GetBoolean(lua_State* lua, int argument, int index)
{
	if (!lua_isboolean(lua, index))
	{
		BadArgumentError(lua, argument, "boolean", GetType(lua, index));
		return false;
	}

	return lua_toboolean(lua, index);
}

double Lua::GetNumber(lua_State* lua, int argument, int index)
{
	if (!lua_isnumber(lua, index))
	{
		BadArgumentError(lua, argument, "number", GetType(lua, index));
		return 0;
	}

	return lua_tonumber(lua, index);
}

const char* Lua::GetString(lua_State* lua, int argument, int index)
{
	if (lua_type(lua, index) != LUA_TSTRING)
	{
		BadArgumentError(lua, argument, "string", GetType(lua, index));
		return "";
	}

	return lua_tostring(lua, index);
}

void* Lua::GetObject(lua_State* lua, const char* className, int argument, int index)
{
	if (!lua_isuserdata(lua, index))
	{
		BadArgumentError(lua, argument, className, GetType(lua, index));
		return nullptr;
	}

	LuaObject* object = (LuaObject*)lua_touserdata(lua, index);

	if (!ReflectionData::GetType(object->typeID)->Inherits(className))
	{
		BadArgumentError(lua, argument, className, object->typeName);
		return nullptr;
	}

	return object->data;
}

int Lua::GetObject(lua_State* lua, const char* className, int* object, int argument, int index)
{
	if (!lua_isuserdata(lua, index))
	{
		BadArgumentError(lua, argument, className, GetType(lua, index));
		return LUA_NOREF;
	}

	if (*object != LUA_NOREF)
		luaL_unref(lua, LUA_REGISTRYINDEX, *object);

	lua_pushvalue(lua, index);
	*object = luaL_ref(lua, LUA_REGISTRYINDEX);

	return *object;
}

int Lua::GetThread(lua_State* lua, int* thread, int argument, int index)
{
	if (!lua_isthread(lua, index))
	{
		BadArgumentError(lua, argument, "thread", GetType(lua, index));
		return LUA_NOREF;
	}

	if (*thread != LUA_NOREF)
		luaL_unref(lua, LUA_REGISTRYINDEX, *thread);

	lua_pushvalue(lua, index);
	*thread = luaL_ref(lua, LUA_REGISTRYINDEX);

	return *thread;
}

int Lua::GetFunction(lua_State* lua, int* function, int argument, int index)
{
	if (!lua_isfunction(lua, index))
	{
		BadArgumentError(lua, argument, "function", GetType(lua, index));
		return LUA_NOREF;
	}

	if (*function != LUA_NOREF)
		luaL_unref(lua, LUA_REGISTRYINDEX, *function);

	lua_pushvalue(lua, index);
	*function = luaL_ref(lua, LUA_REGISTRYINDEX);

	return *function;
}

int Lua::ReleaseThread(lua_State* lua, int* thread)
{
	if (*thread != LUA_NOREF)
		luaL_unref(lua, LUA_REGISTRYINDEX, *thread);

	*thread = LUA_NOREF;

	return *thread;
}

int Lua::ReleaseFunction(lua_State* lua, int* function)
{
	if (*function != LUA_NOREF)
		luaL_unref(lua, LUA_REGISTRYINDEX, *function);

	*function = LUA_NOREF;

	return *function;
}

int Lua::ReleaseObject(lua_State* lua, int* object)
{
	if (*object != LUA_NOREF)
		luaL_unref(lua, LUA_REGISTRYINDEX, *object);

	*object = LUA_NOREF;

	return *object;
}

void Lua::UnpackTable(lua_State* lua, const LuaIterator& iterator, int argument, int index)
{
	if (!lua_istable(lua, index))
	{
		BadArgumentError(lua, argument, "table", lua_typename(lua, index));
		return;
	}

	lua_pushvalue(lua, index);
	lua_pushnil(lua);

	while (lua_next(lua, -2))
	{
		int values = iterator(lua);

		lua_pop(lua, 1 + values);
	}

	lua_pop(lua, 1);
}

void Lua::PackTable(lua_State* lua, const LuaIterator& iterator, int arraySize, int mapSize)
{
	lua_createtable(lua, arraySize, mapSize);

	int values = 0;

	do
	{
		values = iterator(lua);

		for (int i = values; i > 0; --i)
			lua_settable(lua, -i * 2 - 1);

	} while (values > 0);
}

int Lua::CallFunction(lua_State* lua, int function, int arguments, int results)
{
	if (function == LUA_NOREF)
		return 0;

	lua_rawgeti(lua, LUA_REGISTRYINDEX, function);

	if (!lua_isfunction(lua, -1))
	{
		lua_pop(lua, 1);

		return 0;
	}

	lua_insert(lua, -arguments - 1);
	lua_call(lua, arguments, results);

	return results;
}

typedef void(*LuaCallbackFunction)(lua_State*, const DataMember* member, void* data, int index);
typedef std::vector<LuaCallbackFunction> LuaCallbackVector;

void insert(LuaCallbackVector& vector, int id, LuaCallbackFunction callback)
{
	for (int i = int(vector.size()); i <= id; ++i)
		vector.push_back(nullptr);

	vector[id] = callback;
}

template <typename T>
T& castType(void* data)
{
	return *reinterpret_cast<T*>(data);
}

void Lua::ToLua(lua_State* lua, const DataMember* member, void* data, int index)
{
	void* location = member->GetLocation(data);

	if (member->Type->IsClass || member->Type->IsLibrary)
	{
		if (member->Type->Inherits("Object"))
		{
			std::shared_ptr<Engine::Object> object = castType<std::shared_ptr<Engine::Object>>(location);

			if (object == nullptr)
			{
				lua_pushnil(lua);

				return;
			}

			lua_pushstring(lua, "Objects");

			lua_gettable(lua, LUA_REGISTRYINDEX);

			int objectTable = lua_gettop(lua);

			lua_pushnumber(lua, lua_Number(object->GetObjectID()));

			lua_gettable(lua, objectTable);

			if (lua_gettop(lua) == objectTable || lua_isnil(lua, objectTable + 1))
				Engine::MakeLuaReference(lua, object->GetObjectID());
		}
		else if (member->Type->CopyConstructor)
		{
			//Handle<CoreClass> handle;

			void* data = member->Type->CreateRaw();

			member->Type->CopyConstructor(data, location);

			Engine::MakeLuaTypeReference(lua, member->Type, data);
		}
		else
		{
			std::cout << "unimplemented" << std::endl;

			lua_pushnil(lua);
		}
	}
	else if (member->Type->IsEnum)
	{
		
		std::string enumValue = Engine::EnumType::GetType(member->TypeName).Items.find(*((int*)location))->second.Name;//std::string(member->TypeName.c_str(), member->TypeName.size() - 4) + '\0';

		lua_getglobal(lua, "Enum");

		int i = lua_gettop(lua);

		lua_pushstring(lua, member->TypeName.c_str());

		lua_gettable(lua, i);

		i = lua_gettop(lua);

		lua_pushstring(lua, enumValue.c_str());

		lua_gettable(lua, i);
	}
	else
	{
		
		static LuaCallbackVector types;
		static bool initialized = false;

		if (!initialized)
		{
			initialized = true;

			insert(types, coreTypeIDs.Void, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				lua_pushnil(lua);
			});

			insert(types, coreTypeIDs.Boolean, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				lua_pushboolean(lua, castType<bool>(data));
			});

			insert(types, coreTypeIDs.Character, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				lua_pushnumber(lua, lua_Number(castType<char>(data)));
			});

			insert(types, coreTypeIDs.UnsignedCharacter, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				lua_pushnumber(lua, lua_Number(castType<unsigned char>(data)));
			});

			insert(types, coreTypeIDs.Short, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				lua_pushnumber(lua, lua_Number(castType<short>(data)));
			});

			insert(types, coreTypeIDs.UnsignedShort, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				lua_pushnumber(lua, lua_Number(castType<unsigned short>(data)));
			});

			insert(types, coreTypeIDs.Integer, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				lua_pushnumber(lua, lua_Number(castType<int>(data)));
			});

			insert(types, coreTypeIDs.UnsignedInteger, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				lua_pushnumber(lua, lua_Number(castType<unsigned int>(data)));
			});

			insert(types, coreTypeIDs.Long, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				lua_pushnumber(lua, lua_Number(castType<long>(data)));
			});

			insert(types, coreTypeIDs.UnsignedLong, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				lua_pushnumber(lua, lua_Number(castType<unsigned long>(data)));
			});

			insert(types, coreTypeIDs.LongLong, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				lua_pushnumber(lua, lua_Number(castType<long long>(data)));
			});

			insert(types, coreTypeIDs.UnsignedLongLong, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				lua_pushnumber(lua, lua_Number(castType<unsigned long long>(data)));
			});

			insert(types, coreTypeIDs.Float, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				lua_pushnumber(lua, lua_Number(castType<float>(data)));
			});

			insert(types, coreTypeIDs.Double, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				lua_pushnumber(lua, lua_Number(castType<double>(data)));
			});

			insert(types, coreTypeIDs.String, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				lua_pushstring(lua, castType<std::string>(data).c_str());
			});

			insert(types, coreTypeIDs.LuaFunction, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				int id = castType<int>(data);
				
				if (id != LUA_NOREF)
					lua_rawgeti(lua, LUA_REGISTRYINDEX, id);
				else
					lua_pushnil(lua);
			});

			insert(types, coreTypeIDs.LuaThread, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				int thread = castType<int>(data);
			
				if (thread != LUA_NOREF)
					lua_rawgeti(lua, LUA_REGISTRYINDEX, thread);
				else
					lua_pushnil(lua);
			});

			insert(types, coreTypeIDs.LuaTable, [](lua_State* lua, const DataMember* member, void* data, int index)
			{

			});
		}

		if (member->TypeID >= 0 && member->TypeID < int(types.size()))
			types[member->TypeID](lua, member, location, index);
		else
		{
			std::cout << "ToLua: unidentified type '" << member->TypeName << "'";

			lua_pushnil(lua);
		}
	}
}

void Lua::FromLua(lua_State* lua, const DataMember* member, void* data, int index)
{
	void* location = member->GetLocation(data);

	if (member->Type->IsClass || member->Type->IsLibrary)
	{
		if (lua_isuserdata(lua, index))
		{
			if (member->Type->Inherits("Object"))
			{
				std::shared_ptr<Engine::Object>& object = castType<std::shared_ptr<Engine::Object>>(location);
				
				Engine::LuaData* data = (Engine::LuaData*)lua_topointer(lua, index);

				object = data->Reference->Cast<Engine::Object>();
			}
			else if (member->Type->AssignmentOperator)
			{
				Engine::LuaData* object = (Engine::LuaData*)lua_topointer(lua, index);

				member->Type->AssignmentOperator(location, object->Data);
			}
			else
			{
				std::cout << "unimplemented" << std::endl;

				lua_pushnil(lua);
			}
		}
		else if (member->Type->Inherits("Object") && lua_isnil(lua, index))
			castType<std::shared_ptr<Engine::Object>>(location).reset();
		else
		{
			SetErrorMessage("Attempt to set member '" + member->Name + "' of '" + member->ParentName + "', expected '" + member->TypeName + "' got '" + std::string(GetType(lua, index)) + "'");
			Lua::Error(lua);
		}
	}
	else if (member->Type->IsEnum)
	{
		int& value = castType<int>(location);
		const Engine::EnumType& type = Engine::EnumType::GetType(member->TypeName);

		if (lua_isnumber(lua, index))
		{
			value = int(lua_tonumber(lua, index));
		}
		else if (lua_isuserdata(lua, index))
		{
			Engine::LuaData* data = (Engine::LuaData*)lua_topointer(lua, index);

			if (data->Meta != nullptr && data->Meta->IsEnum && data->Meta->ParentData == member->Type)
				value = *((int*)data->Meta->GetMember("Value")->GetLocation(nullptr));
			else
			{
				SetErrorMessage("Attempt to set member '" + member->Name + "' of '" + member->ParentName + "', expected '" + member->TypeName + "' got '" + std::string(GetType(lua, index)) + "'");
				Lua::Error(lua);
			}
		}
		else if (lua_type(lua, index) == LUA_TSTRING)
		{
			const char* item = lua_tostring(lua, index);

			Engine::EnumType::EnumItemNameMap::const_iterator i = type.ItemNames.find(item);

			if (i != type.ItemNames.end())
				value = int(type.Items.find(i->second)->second.Value);
			else
			{
				Lua::SetErrorMessage("Attempt to use invalid enum item '" + std::string(item) + "' of enum type '" + member->TypeName + "'");
				Lua::Error(lua);
			}
		}
		else
		{
			SetErrorMessage("Attempt to set member '" + member->Name + "' of '" + member->ParentName + "', expected '" + member->TypeName + "' got '" + std::string(GetType(lua, index)) + "'");
			Lua::Error(lua);
		}
	}
	else
	{

		static LuaCallbackVector types;
		static bool initialized = false;

		if (!initialized)
		{
			initialized = true;

			insert(types, coreTypeIDs.Void, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				// nope
				std::cout << "why you do this" << std::endl;
			});

			insert(types, coreTypeIDs.Boolean, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				bool& value = castType<bool>(data);

				if (lua_isboolean(lua, index))
					value = lua_toboolean(lua, index);
				else
					Lua::BadArgumentError(lua, index - 1, "bool", GetType(lua, index));
			});

			insert(types, coreTypeIDs.Character, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				char& value = castType<char>(data);

				if (lua_isnumber(lua, index))
					value = char(lua_tonumber(lua, index));
				else
					Lua::BadArgumentError(lua, index - 1, "char", GetType(lua, index));
			});

			insert(types, coreTypeIDs.UnsignedCharacter, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				unsigned char& value = castType<unsigned char>(data);

				if (lua_isnumber(lua, index))
					value = (unsigned char)(lua_tonumber(lua, index));
				else
					Lua::BadArgumentError(lua, index - 1, "unsigned char", GetType(lua, index));
			});

			insert(types, coreTypeIDs.Short, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				short& value = castType<short>(data);

				if (lua_isnumber(lua, index))
					value = short(lua_tonumber(lua, index));
				else
					Lua::BadArgumentError(lua, index - 1, "short", GetType(lua, index));
			});

			insert(types, coreTypeIDs.UnsignedShort, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				unsigned short& value = castType<unsigned short>(data);

				if (lua_isnumber(lua, index))
					value = (unsigned short)(lua_tonumber(lua, index));
				else
					Lua::BadArgumentError(lua, index - 1, "unsigned short", GetType(lua, index));
			});

			insert(types, coreTypeIDs.Integer, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				int& value = castType<int>(data);

				if (lua_isnumber(lua, index))
					value = int(lua_tonumber(lua, index));
				else
					Lua::BadArgumentError(lua, index - 1, "int", GetType(lua, index));
			});

			insert(types, coreTypeIDs.UnsignedInteger, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				unsigned int& value = castType<unsigned int>(data);

				if (lua_isnumber(lua, index))
					value = (unsigned int)(lua_tonumber(lua, index));
				else
					Lua::BadArgumentError(lua, index - 1, "unsigned int", GetType(lua, index));
			});

			insert(types, coreTypeIDs.Long, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				long& value = castType<long>(data);

				if (lua_isnumber(lua, index))
					value = long(lua_tonumber(lua, index));
				else
					Lua::BadArgumentError(lua, index - 1, "long", GetType(lua, index));
			});

			insert(types, coreTypeIDs.UnsignedLong, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				unsigned long& value = castType<unsigned long>(data);

				if (lua_isnumber(lua, index))
					value = (unsigned long)(lua_tonumber(lua, index));
				else
					Lua::BadArgumentError(lua, index - 1, "unsigned long", GetType(lua, index));
			});

			insert(types, coreTypeIDs.LongLong, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				long long& value = castType<long long>(data);

				if (lua_isnumber(lua, index))
					value = (long long)(lua_tonumber(lua, index));
				else
					Lua::BadArgumentError(lua, index - 1, "long long", GetType(lua, index));
			});

			insert(types, coreTypeIDs.UnsignedLongLong, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				unsigned long long& value = castType<unsigned long long>(data);

				if (lua_isnumber(lua, index))
					value = (unsigned long long)(lua_tonumber(lua, index));
				else
					Lua::BadArgumentError(lua, index - 1, "unsigned long long", GetType(lua, index));
			});

			insert(types, coreTypeIDs.Float, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				float& value = castType<float>(data);

				if (lua_isnumber(lua, index))
					value = float(lua_tonumber(lua, index));
				else
					Lua::BadArgumentError(lua, index - 1, "float", GetType(lua, index));
			});

			insert(types, coreTypeIDs.Double, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				double& value = castType<double>(data);

				if (lua_isnumber(lua, index))
					value = double(lua_tonumber(lua, index));
				else
					Lua::BadArgumentError(lua, index - 1, "double", GetType(lua, index));
			});

			insert(types, coreTypeIDs.String, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
				std::string& value = castType<std::string>(data);

				if (lua_type(lua, index) == LUA_TSTRING)
					value = lua_tostring(lua, index);
				else
					Lua::BadArgumentError(lua, index - 1, "string", GetType(lua, index));
			});

			insert(types, coreTypeIDs.LuaFunction, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
			});

			insert(types, coreTypeIDs.LuaThread, [](lua_State* lua, const DataMember* member, void* data, int index)
			{
			});

			insert(types, coreTypeIDs.LuaTable, [](lua_State* lua, const DataMember* member, void* data, int index)
			{

			});
		}

		if (member->TypeID >= 0 && member->TypeID < int(types.size()))
			types[member->TypeID](lua, member, location, index);
		else
			std::cout << "ToLua: unidentified type '" << member->TypeName << "'";
	}
}
