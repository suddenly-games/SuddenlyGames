#pragma once

#include "ReflectionData.h"


class LibraryData : public ReflectionData
{
public:
	LibraryData(const std::string& name, const std::string& description = std::string());

	void Copy(void* object, void* data) const;
	std::string Format(void* object) const;
	void Parse(void* object, const std::string& data) const;
};

struct lua_State;

class ClassData : public ReflectionData
{
public:
	class Property
	{
	public:
		typedef const ReflectionData::FunctionOverload<int, lua_State*>* LuaFunctionOverload;
		typedef std::vector<LuaFunctionOverload> FunctionVector;

		const ReflectionData* ParentType = nullptr;

		std::string Name;
		std::string Description;

		LuaFunctionOverload Getter = nullptr;
		FunctionVector Setters;

		void AddGetter(const std::string& name, const std::string& typeName);
		void AddSetter(const std::string& name, const std::string& typeName);
	};

	typedef std::map<std::string, Property> PropertyMap;
	typedef std::vector<std::string> StringVector;

	ClassData(const std::string& name, const std::string& description, const std::string& inherits);
	ClassData(const std::string& name, const std::string& description = std::string(), const ReflectionData* inherits = nullptr);
	virtual ~ClassData() {}

	void Copy(void* object, void* data) const;
	std::string Format(void* object) const;
	void Parse(void* object, const std::string& data) const;

	Property* RegisterProperty(const std::string& name);
	const StringVector& GetProperties() const;
	const Property* GetProperty(const std::string& name) const;
	void LoadProperties(const ClassData* parent);

private:
	PropertyMap Properties;
	StringVector PropertyList;

	void LoadInheritedData();
};

class TypeData : public ClassData
{
public:
	TypeData(const std::string& name, const std::string& description, const std::string& inherits) : ClassData(name, description, inherits) {}
	TypeData(const std::string& name, const std::string& description = std::string(), const ReflectionData* inherits = nullptr) : ClassData(name, description, inherits) {}
	virtual ~TypeData() {}

};

class VoidData : public ReflectionData
{
public:
	VoidData() : ReflectionData(std::string("void")) {}

	void Copy(void* object, void* data) const {}
	std::string Format(void* object) const { return ""; }
	void Parse(void* object, const std::string& data) const {}

	static void Initialize();
};

class BooleanData : public ReflectionData
{
public:
	BooleanData() : ReflectionData(std::string("bool")) {}

	void Copy(void* object, void* data) const;
	std::string Format(void* object) const;
	void Parse(void* object, const std::string& data) const;

	static void Initialize();
};

class CharacterData : public ReflectionData
{
public:
	CharacterData() : ReflectionData(std::string("char")) {}

	void Copy(void* object, void* data) const;
	std::string Format(void* object) const;
	void Parse(void* object, const std::string& data) const;

	static void Initialize();
};

class UnsignedCharacterData : public ReflectionData
{
public:
	UnsignedCharacterData() : ReflectionData(std::string("unsigned char")) {}

	void Copy(void* object, void* data) const;
	std::string Format(void* object) const;
	void Parse(void* object, const std::string& data) const;

	static void Initialize();
};

class ShortData : public ReflectionData
{
public:
	ShortData() : ReflectionData(std::string("short")) {}

	void Copy(void* object, void* data) const;
	std::string Format(void* object) const;
	void Parse(void* object, const std::string& data) const;

	static void Initialize();
};

class UnsignedShortData : public ReflectionData
{
public:
	UnsignedShortData() : ReflectionData(std::string("unsigned short")) {}

	void Copy(void* object, void* data) const;
	std::string Format(void* object) const;
	void Parse(void* object, const std::string& data) const;

	static void Initialize();
};

class IntegerData : public ReflectionData
{
public:
	IntegerData() : ReflectionData(std::string("int")) {}

	void Copy(void* object, void* data) const;
	std::string Format(void* object) const;
	void Parse(void* object, const std::string& data) const;

	static void Initialize();
};

class UnsignedIntegerData : public ReflectionData
{
public:
	UnsignedIntegerData() : ReflectionData(std::string("unsigned int")) {}

	void Copy(void* object, void* data) const;
	std::string Format(void* object) const;
	void Parse(void* object, const std::string& data) const;

	static void Initialize();
};

class LongIntegerData : public ReflectionData
{
public:
	LongIntegerData() : ReflectionData(std::string("long")) {}

	void Copy(void* object, void* data) const;
	std::string Format(void* object) const;
	void Parse(void* object, const std::string& data) const;

	static void Initialize();
};

class UnsignedLongIntegerData : public ReflectionData
{
public:
	UnsignedLongIntegerData() : ReflectionData(std::string("unsigned long")) {}

	void Copy(void* object, void* data) const;
	std::string Format(void* object) const;
	void Parse(void* object, const std::string& data) const;

	static void Initialize();
};

class LongLongIntegerData : public ReflectionData
{
public:
	LongLongIntegerData() : ReflectionData(std::string("long long")) {}

	void Copy(void* object, void* data) const;
	std::string Format(void* object) const;
	void Parse(void* object, const std::string& data) const;

	static void Initialize();
};

class UnsignedLongLongIntegerData : public ReflectionData
{
public:
	UnsignedLongLongIntegerData() : ReflectionData(std::string("unsigned long long")) {}

	void Copy(void* object, void* data) const;
	std::string Format(void* object) const;
	void Parse(void* object, const std::string& data) const;

	static void Initialize();
};

class FloatData : public ReflectionData
{
public:
	FloatData() : ReflectionData(std::string("float")) {}

	void Copy(void* object, void* data) const;
	std::string Format(void* object) const;
	void Parse(void* object, const std::string& data) const;

	static void Initialize();
};

class DoubleData : public ReflectionData
{
public:
	DoubleData() : ReflectionData(std::string("double")) {}

	void Copy(void* object, void* data) const;
	std::string Format(void* object) const;
	void Parse(void* object, const std::string& data) const;

	static void Initialize();
};

class StringData : public ReflectionData
{
public:
	StringData() : ReflectionData(std::string("string")) {}

	void Copy(void* object, void* data) const;
	std::string Format(void* object) const;
	void Parse(void* object, const std::string& data) const;

	static void Initialize();
};
