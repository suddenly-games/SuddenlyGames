#pragma once

#include <memory>

#include "ReflectionData.h"
#include "CoreTypeReflection.h"
#include "PageAllocator.h"
#include "LuaStackTypes.h"
#include "Enum.h"
#include "LuaBinding.h"

#include "Lua.h"

struct lua_State;

/*
Macros:

Init_Reflection(className)

Instantiable
NonInstantiable
No_Reference

Reflected(className)
Reflected_Type(typeName)
Reflect(className)
Reflect_Inherited(className, baseName)
Reflect_Type(className)

Document_Class(string documentation)

Restrict_Siblings
Require_Siblings(types)

Static
Archivable
Document(string documentation)
Class_Member(memberType, memberName)

BindFunction(name, ...)
Function_Overload(...)
ReturnsNothing
Overload_Returns(type)
Overload_Parameters(...)
Function_Parameter(type, name)
Function_Parameter_Default(type, name, value)
Bind_Parameters(name, args)
Bind_Parameters_No_Return(name, args)
*/

struct FunctionBinding : public ReflectionData::FunctionOverload<int, lua_State*>
{
	FunctionBinding(const std::string& documentation, bool isStatic)
	{
		Description = documentation;
		IsStatic = isStatic;
	}

	template <typename T>
	static T* GetBoundObject(lua_State* lua)
	{
		if (lua_isuserdata(lua, 1))
			return (T*)(GetData(const_cast<void*>(lua_topointer(lua, 1))));
		else
			return nullptr;
	}

private:
	static void* GetData(void* data);
};

struct FunctionOverloads : public ReflectionData::Function
{
	int GetOverload(lua_State* lua) const;

	typedef int(*LuaCallback) (lua_State*);

	LuaCallback Callback = nullptr;

	void Add(FunctionBinding* overload)
	{
		overload->Name = Name;
		Overloads.push_back(overload);
	}
};

template <typename T>
class LuaEnum
{
public:
	typedef T Type;

	Type Value;

	LuaEnum(const Type& input) : Value(input) {}

	operator Type() { return Value; }
};

template <typename T>
std::string fetchInherited()
{
	return T::GetTypeName();
}

template <>
std::string fetchInherited<ClassData>();

/*
===========================================================================================
*/

#define Reflect_Enum(name, ...)\
{\
	Engine::CoreTypes<LuaEnum<name>>::RegisterTypeName(#name);\
	\
	Engine::EnumType& enumData = Engine::EnumType::AddType(#name);\
	\
	typedef Enum:: ## name Enum_Type;\
	const char* description = "";\
	\
	__VA_ARGS__\
	\
	enumData.InitReflection();\
}

/*
===========================================================================================
*/

#define Declare_Enum(name) namespace ReflectedEnums { void InitializeEnum ## name(); }

/*
===========================================================================================
*/

#define Enum_Definition(name, ...) namespace ReflectedEnums\
{\
	typedef Enum:: ## name name ;\
	\
	void InitializeEnum ## name()\
	Reflect_Enum(name, __VA_ARGS__);\
}

/*
===========================================================================================
*/

#define Register_Enum(name) ReflectedEnums::InitializeEnum ## name ## ();

/*
===========================================================================================
*/

#define Document_Enum(text) enumData.Description = text; enumData.Description += '\0';

/*
===========================================================================================
*/

#define Document_Item(text) description = text; description += '\0';

/*
===========================================================================================
*/

#define Enum_Item(name) enumData.AddItem(#name, Enum_Type::name, description); description = "";

/*
===========================================================================================
*/

#define Append_Ref(name) name

/*
===========================================================================================
*/

#define NonInstantiable private: static bool IsInstantiable() { return false; }\
template <typename T>\
static std::shared_ptr<Engine::ObjectBase> Allocate()\
{\
	throw std::string("Attempt to create non-instantiable type ") + GetClassMetaData()->Name;\
}

/*
===========================================================================================
*/

#define Instantiable private: static bool IsInstantiable() { return true; }\
template <typename T>\
static std::shared_ptr<Engine::ObjectBase> Allocate()\
{\
	return GameObjectAllocator<T>::Create();\
}

/*
===========================================================================================
*/

#define No_Reference allowReferences = false;

/*
===========================================================================================
*/

#define Restrict_Siblings SiblingsRestricted = true;

/*
===========================================================================================
*/

#define Require_Siblings(...) SiblingRequirements = { ##__VA_ARGS__ };

/*
===========================================================================================
*/

#define Base_Class public: typedef ClassData InheritedData;
#define Inherits_Class(className) public: typedef className::ObjectData InheritedData;
#define Inherits_Type(className) public: typedef className::TypeData InheritedData;

/*
===========================================================================================
*/

#define Document_Library(description) std::string documentation = description;

/*
===========================================================================================
*/

#define Reflect_Library(libraryName, libraryDocumentation, ...)\
void Initialize ## libraryName()\
{\
	static bool initialized = false;\
	\
	if (initialized)\
		return;\
	\
	initialized = true;\
	\
	libraryDocumentation;\
	\
	LibraryData* Data = new LibraryData(libraryName, documentation);\
	\
	__VA_ARGS__\
}

/*
===========================================================================================
*/

#define Reflected(className)\
public:\
	virtual MetaData* GetMetaData() const;\
	\
	virtual void InitializeBase();\
	\
	virtual void UpdateBase(float delta);\
	\
	virtual bool RestrictsSiblings() const\
	{\
		return SiblingsRestricted;\
	}\
	\
	virtual const StringVector& GetRequirements() const\
	{\
		return SiblingRequirements;\
	}\
	\
	class ObjectData : public InheritedData\
	{\
	public:\
		static std::string GetTypeName() { return #className; }\
		\
		typedef className BoundClass;\
		\
		static MetaData* GetMetaData() { return Data; }\
		\
		static void Initialize();\
		\
		static std::shared_ptr<Engine::ObjectBase> Create()\
		{\
			return Allocate<className>();\
		}\
		\
	protected:\
		\
		ObjectData();\
		ObjectData(const std::string& name, const std::string& inherits = #className);\
	private:\
		static ObjectData* Data;\
	};\
	\
	static MetaData* GetClassMetaData() { return ObjectData::GetMetaData(); }\
	\
	className();\
	\
private:\
	static StringVector SiblingRequirements;\
	static bool SiblingsRestricted;\
	\
protected:\
	className(const std::string& name);

/*
===========================================================================================
*/

#define Reflect(className, ...)\
Init_Reflection(className);\
\
void className::InitializeBase()\
{\
	className::Initialize();\
}\
\
void className::UpdateBase(float delta)\
{\
	className::Update(delta);\
}\
\
void className::ObjectData::Initialize()\
{\
	if (Data != nullptr)\
		return;\
	\
	RegisterFactoryFunction(#className, Create);\
	\
	className* object = nullptr;\
	std::string documentation = "";\
	bool isStatic = false;\
	bool isArchivable = false;\
	bool isReadOnly = false;\
	\
	Data = new ObjectData();\
	\
	__VA_ARGS__\
	\
}\
\
className::ObjectData::ObjectData() : ClassData(#className, "", nullptr) {}\
className::ObjectData::ObjectData(const std::string& name, const std::string& inherits) : ClassData(name, "", inherits) {}\
\
className::className() : Name(#className) {}\
className::className(const std::string& name) : Name(name) {}

/*
===========================================================================================
*/

#define Reflect_Inherited(className, baseName, ...)\
using Engine::ObjectBase;\
\
Init_Reflection(className);\
\
void className::InitializeBase()\
{\
	baseName::InitializeBase();\
	className::Initialize();\
}\
\
void className::UpdateBase(float delta)\
{\
	baseName::UpdateBase(delta);\
	className::Update(delta);\
}\
\
void className::ObjectData::Initialize()\
{\
	if (Data != nullptr)\
		return;\
	\
	Engine::CoreTypes<std::shared_ptr<className>>::RegisterTypeName(#className);\
	\
	RegisterFactoryFunction(#className, Create);\
	\
	className* object = nullptr;\
	std::string documentation = "";\
	bool isStatic = false;\
	bool isArchivable = false;\
	bool isReadOnly = false;\
	\
	Data = new ObjectData(#className, #baseName);\
	\
	Data->LoadProperties(reinterpret_cast<const ClassData*>(Data->ParentData));\
	\
	__VA_ARGS__\
	\
}\
\
className::ObjectData::ObjectData() : baseName::ObjectData(#className, #baseName) {}\
className::ObjectData::ObjectData(const std::string& name, const std::string& inherits) : baseName::ObjectData(name, inherits) {}\
\
className::className() : baseName(#className) {}\
className::className(const std::string& name) : baseName(name) {}

/*
===========================================================================================
*/

#define Reflected_Type(className)\
public:\
	typedef className BoundClass;\
	\
	static MetaData* GetMetaData()\
	{\
		return TypeData::GetMetaData();\
	}\
	\
	class TypeData : public ClassData\
	{\
	public:\
		static std::string GetTypeName();\
		\
		typedef className BoundClass; \
		\
		static MetaData* GetMetaData() { return Data; }\
		\
		static void Initialize();\
		\
	private:\
		static TypeData* Data;\
		static BaseAllocator& Allocator;\
		\
		static std::string GetInherited();\
		\
		TypeData() : ClassData(#className, GetInherited(), nullptr) {}\
	};

/*
===========================================================================================
*/

#define Reflect_Type(typeName, ...)\
Init_Type_Reflection(typeName);\
\
std::string typeName::TypeData::GetInherited()\
{\
	return fetchInherited<InheritedData>();\
}\
\
std::string typeName::TypeData::GetTypeName()\
{\
	return Engine::CoreTypes<typeName>::GetTypeName();\
}\
\
void typeName::TypeData::Initialize()\
{\
	if (Data != nullptr)\
		return;\
	\
	typeName* object = nullptr;\
	std::string documentation = "";\
	bool isStatic = false;\
	bool isArchivable = false;\
	bool isReadOnly = false;\
	bool allowReferences = true;\
	\
	Data = new TypeData();\
	\
	Data->Create = [] () -> std::shared_ptr<Engine::ObjectBase>\
	{\
		return nullptr;/*std::shared_ptr<typeName>(reinterpret_cast<typeName*>(typeName::TypeData::Allocator.Allocate()), [](typeName* data)*/\
		/*{*/\
			/*typeName::TypeData::Allocator.Destroy<typeName>(data);*/\
		/*});*/\
	};\
	\
	Data->CreateRaw = [] () -> void*\
	{\
		return typeName::TypeData::Allocator.Allocate();\
	};\
	\
	Data->Delete = [] (void* data)\
	{\
		reinterpret_cast<typeName*>(data)->~typeName();\
		\
		typeName::TypeData::Allocator.Free(data);\
	};\
	\
	Data->CopyConstructor = [] (void* object, void* data)\
	{\
		new (reinterpret_cast<typeName*>(object)) typeName(*reinterpret_cast<typeName*>(data));\
	};\
	\
	Data->AssignmentOperator = [] (void* object, void* data)\
	{\
		*reinterpret_cast<typeName*>(object) = *reinterpret_cast<typeName*>(data);\
	};\
	\
	__VA_ARGS__\
	\
}

/*
===========================================================================================
*/

#define Init_Reflection(ClassA) \
\
MetaData* ClassA::GetMetaData() const\
{\
	return ObjectData::GetMetaData();\
}\
\
ClassA::ObjectData* ClassA::ObjectData::Data = nullptr;\
ObjectBase::StringVector ClassA::SiblingRequirements = StringVector();\
bool ClassA::SiblingsRestricted = false;

/*
===========================================================================================
*/

#define Init_Type_Reflection(ClassA)\
namespace\
{\
	PageAllocator<sizeof(ClassA) + sizeof(void*), 0x10000> ClassA ## _Allocator;\
}\
typename ClassA::TypeData* ClassA::TypeData::Data = nullptr;\
BaseAllocator& ClassA::TypeData::Allocator = ClassA ## _Allocator;

/*
===========================================================================================
*/

#define Document_Class(documentation) Data->Description = documentation; Data->Description += '\0';

/*
===========================================================================================
*/

#define Define_Type(typeName, conversions, returns)\
namespace LuaTypes\
{\
	class Lua_ ## typeName\
	{\
	public:\
		template <typename T>\
		class Converter\
		{\
		public:\
			Converter(int argumentNumber, int& arguments, bool isStatic, const T& defaultValue = T()) : ArgumentNumber(argumentNumber), Arguments(arguments), IsStatic(isStatic), DefaultValue(defaultValue) {}\
			\
			lua_State** LuaState = nullptr;\
			int ArgumentNumber = 0;\
			int& Arguments = ArgumentNumber;\
			bool HasDefaultValue = false;\
			bool IsStatic = false;\
			T DefaultValue = T();\
			\
			conversions\
		};\
		\
		class ReturnValue\
		{\
		public:\
			const int ReturnValues = 1;\
			\
			lua_State** LuaState = nullptr;\
			\
			returns\
		};\
	};\
}\
\
template<>\
class CoreTypes<typeName>\
{\
public:\
	typedef LuaTypes::Lua_ ## typeName LuaType;\
	\
	static const std::string GetTypeName() { return #typeName; };\
};

/*
===========================================================================================
*/

#define Define_Value_Type(typeName)\
Define_Type(typeName,\
	Lua_Conversions\
	(\
		Conversion_Operation\
		{\
			static const ReflectionData* type = nullptr;\
			\
			if (type == nullptr)\
				type = ReflectionData::GetType(#typeName);\
			\
			int index = ArgumentNumber + (IsStatic ? 1 : 2);\
			\
			if (lua_isuserdata(*LuaState, index))\
			{\
				LuaData* object = (LuaData*)lua_topointer(*LuaState, index);\
				\
				if (object->Data == nullptr)\
				{\
					Lua::SetErrorMessage("Attempt to pass in static object to type '" + std::string(#typeName) + "'");\
					Lua::Error(*LuaState);\
				}\
				else if (object->Meta != type)\
					Lua::BadArgumentError(*LuaState, ArgumentNumber + 1, #typeName, object->Meta->Name.c_str());\
				\
				return *reinterpret_cast<typeName*>(object->Data);/*ObjectReference(object->Reference).GetObjectData<typeName>();*/\
			}\
			else\
				Lua::BadArgumentError(*LuaState, ArgumentNumber + 1, #typeName, Lua::GetType(*LuaState, index));\
			\
			throw "shut up, compiler";\
		}\
	),\
	Lua_Returns\
	(\
		Return_Operation(const typeName& value)\
		{\
			static const ReflectionData* type = nullptr;\
			\
			if (type == nullptr)\
				type = ReflectionData::GetType(#typeName);\
			\
			try\
			{\
				void* data = type->CreateRaw();\
				\
				new (data) typeName(value);\
				\
				MakeLuaTypeReference(*LuaState, type, data);\
			}\
			catch (std::string& err)\
			{\
				Lua::SetErrorMessage(err);\
				Lua::Error(*LuaState);\
			}\
		}\
	)\
);

/*
===========================================================================================
*/

#define Lua_Conversions(...) __VA_ARGS__
#define Lua_Returns(...) __VA_ARGS__

/*
===========================================================================================
*/

#define Conversion_Operation operator T() const
#define Return_Operation void operator()

/*
===========================================================================================
*/

#define Read_Only isReadOnly = true;
#define Archivable isArchivable = true;
#define Static isStatic = true;
#define Document(...) documentation = __VA_ARGS__; documentation += '\0';

/*
===========================================================================================
*/

#define Class_Member(memberType, memberName) Data->AddMember(#memberName, Engine::CoreTypes<memberType>::GetTypeName(), &object->memberName, documentation, isArchivable, isStatic);\
documentation = "";\
isStatic = false;\
isArchivable = false;\
isReadOnly = false;

/*
===========================================================================================
*/

#define Library_Member(memberType, memberName) Data->AddMember(#memberName, Engine::CoreTypes<memberType>::GetTypeName(), &memberName, documentation, isArchivable, isStatic);\
documentation = "";\
isStatic = false;\
isArchivable = false;\
isReadOnly = false;

/*
===========================================================================================
*/

#define Bind_Function(funcName, ...) {\
	static FunctionOverloads binding;\
	\
	binding.Name = #funcName;\
	binding.ParentName = GetTypeName();\
	\
	__VA_ARGS__\
	\
	binding.Callback = [] (lua_State* lua) -> int\
	{\
		if (binding.Overloads.size() == 1)\
			return binding.GetCallback<int, lua_State*>(0)(lua);\
		\
		int overload = binding.GetOverload(lua);\
		\
		if (overload >= 0 && overload < int(binding.Overloads.size()))\
			return binding.GetCallback<int, lua_State*>(overload)(lua);\
		\
		if (binding.Overloads.size() == 0)\
		{\
			Lua::SetErrorMessage("Function '" + binding.Name + "' of type '" + binding.ParentName + "' has 0 overloads");\
			Lua::Error(lua);\
		}\
		else\
			Lua::BadArgumentError(lua, 1, binding.Overloads[0]->Parameters[0].TypeName.c_str(), Lua::GetType(lua, 2));\
		\
		return 0;\
	};\
	Data->AddFunction(binding.Name, &binding);\
}

/*
===========================================================================================
*/

#define Function_Overload(...) {\
	struct FunctionBindingOverload : public FunctionBinding\
	{\
		FunctionBindingOverload(const std::string& overloadDocumentation, bool isStatic) : FunctionBinding(overloadDocumentation, isStatic)\
		{\
			std::string documentation;\
			int argumentNumber = 0;\
			static int arguments = 0;\
			ParentName = GetTypeName();\
			static bool IsStatic = isStatic;\
			static lua_State* currentLua = nullptr;\
			\
			__VA_ARGS__\
		}\
	};\
	binding.Add(new FunctionBindingOverload(documentation, isStatic));\
}\
documentation = "";\
isStatic = false;

/*
===========================================================================================
*/

#define Returns_Nothing ReturnTypeName = "void";\
static Engine::LuaTypes::Lua_nil::ReturnValue returnOperation;\
returnOperation.LuaState = &currentLua;

/*
===========================================================================================
*/

#define Overload_Returns(type) ReturnTypeName = Engine::CoreTypes<type>::GetTypeName();\
ReturnValueDescription = documentation;\
static Engine::CoreTypes<type>::LuaType::ReturnValue returnOperation;\
returnOperation.LuaState = &currentLua;\
documentation = "";

/*
===========================================================================================
*/

#define Overload_Parameters(...) __VA_ARGS__

/*
===========================================================================================
*/

#define Function_Parameter(type, name)\
static Engine::CoreTypes<type>::LuaType::Converter<type> name = Engine::CoreTypes<type>::LuaType::Converter<type>(argumentNumber, arguments, isStatic);\
name.LuaState = &currentLua;\
Parameters.push_back(ParameterData ( documentation, #name, "",Engine:: CoreTypes<type>::GetTypeName(), nullptr, false ));\
Append_Ref(name).HasDefaultValue = false;\
++argumentNumber;\
++arguments;\
documentation = "";

/*
===========================================================================================
*/

#define Function_Parameter_Default(type, name, value)\
static Engine::CoreTypes<type>::LuaType::Converter<type> name = Engine::CoreTypes<type>::LuaType::Converter<type>(argumentNumber, arguments, isStatic, value);\
name.LuaState = &currentLua;\
name.HasDefaultValue = true;\
Parameters.push_back(ParameterData ( documentation, #name, #value, Engine::CoreTypes<type>::GetTypeName(), nullptr, true ));\
++argumentNumber;\
++arguments;\
documentation = "";

/*
===========================================================================================
*/

#define Bind_Parameters(funcName, ...)\
Callback = [](lua_State* lua) -> int\
{\
	currentLua = lua;\
	\
	BoundClass* luaObject = GetBoundObject<BoundClass>(lua);\
	\
	try\
	{\
		if (luaObject == nullptr)\
			throw "Attempt to use Lua method '" #funcName "' from class '" + GetTypeName() + "' on nil object. Did you mean to use ':' instead of '.'?";\
		\
		returnOperation(luaObject->funcName(__VA_ARGS__));\
	}\
	catch(std::string& error)\
	{\
		Lua::SetErrorMessage(error);\
		Lua::Error(lua);\
	}\
	\
	return returnOperation.ReturnValues;\
};

/*
===========================================================================================
*/

#define Static_Bind_Parameters(funcName, ...)\
Callback = [](lua_State* lua) -> int\
{\
	currentLua = lua;\
	\
	try\
	{\
		returnOperation(BoundClass::funcName(__VA_ARGS__));\
	}\
	catch(std::string& error)\
	{\
		Lua::SetErrorMessage(error);\
		Lua::Error(lua);\
	}\
	\
	return returnOperation.ReturnValues;\
};

/*
===========================================================================================
*/

#define Library_Bind_Parameters(funcName, ...)\
Callback = [](lua_State* lua) -> int\
{\
	currentLua = lua;\
	\
	BoundClass* luaObject = GetBoundObject<BoundClass>(lua);\
	\
	try\
	{\
		if (luaObject == nullptr)\
			throw "Attempt to use Lua method '" #funcName "' from class '" + GetTypeName() + "' on nil object. Did you mean to use ':' instead of '.'?";\
		\
		returnOperation(luaObject->funcName(__VA_ARGS__));\
	}\
	catch(std::string& error)\
	{\
		Lua::SetErrorMessage(error);\
		Lua::Error(lua);\
	}\
	\
	return returnOperation.ReturnValues;\
};

/*
===========================================================================================
*/

#define Bind_Parameters_No_Return(funcName, ...)\
Callback = [](lua_State* lua) -> int\
{\
	currentLua = lua;\
	\
	BoundClass* luaObject = GetBoundObject<BoundClass>(lua);\
	\
	try\
	{\
		if (luaObject == nullptr)\
			throw "Attempt to use Lua method '" #funcName "' from class '" + GetTypeName() + "' on nil object. Did you mean to use ':' instead of '.'?";\
		\
		luaObject->funcName(__VA_ARGS__);\
		returnOperation();\
	}\
	catch (std::string& error)\
	{\
		Lua::SetErrorMessage(error); \
		Lua::Error(lua); \
	}\
	\
	return returnOperation.ReturnValues;\
};

/*
===========================================================================================
*/

#define Static_Bind_Parameters_No_Return(funcName, ...)\
Callback = [](lua_State* lua) -> int\
{\
	currentLua = lua;\
	\
	try\
	{\
		BoundClass::funcName(__VA_ARGS__);\
		returnOperation();\
	}\
	catch(std::string& error)\
	{\
		Lua::SetErrorMessage(error);\
		Lua::Error(lua);\
	}\
	\
	return returnOperation.ReturnValues;\
};

/*
===========================================================================================
*/

#define Library_Bind_Parameters_No_Return(funcName, ...)\
Callback = [](lua_State* lua) -> int\
{\
	currentLua = lua;\
	\
	try\
	{\
		funcName(__VA_ARGS__);\
		returnOperation();\
	}\
	catch(std::string& error)\
	{\
		Lua::SetErrorMessage(error);\
		Lua::Error(lua);\
	}\
	\
	return returnOperation.ReturnValues;\
};

/*
===========================================================================================
*/

#define Bind_Constructor(...)\
Callback = [](lua_State* lua) -> int\
{\
	currentLua = lua;\
	\
	BoundClass* luaObject = GetBoundObject<BoundClass>(lua);\
	\
	try\
	{\
		new (luaObject) BoundClass(__VA_ARGS__);\
		returnOperation();\
	}\
	catch(std::string& error)\
	{\
		Lua::SetErrorMessage(error);\
		Lua::Error(lua);\
	}\
	\
	return returnOperation.ReturnValues;\
};

/*
===========================================================================================
*/

#define Bind_Lua_Function(funcName)\
Callback = [](lua_State* lua) -> int\
{\
	currentLua = lua;\
	\
	BoundClass* luaObject = GetBoundObject<BoundClass>(lua);\
	\
	return luaObject->funcName(lua);\
};

/*
===========================================================================================
*/

#define Register_Lua_Property(propertyName, ...)\
{\
	ClassData::Property* property = Data->RegisterProperty(#propertyName);\
	\
	property->Description = documentation;\
	\
	__VA_ARGS__\
}\
\
documentation = ""; \

/*
===========================================================================================
*/

#define Property_Getter(getterName, inputType)\
property->AddGetter(#getterName, Engine::CoreTypes<inputType>::GetTypeName());

/*
===========================================================================================
*/

#define Property_Setters(...) __VA_ARGS__

/*
===========================================================================================
*/

#define Bind_Setter(setterName, inputType)\
property->AddSetter(#setterName, Engine::CoreTypes<inputType>::GetTypeName());
