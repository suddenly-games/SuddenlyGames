#include "CoreTypeReflection.h"

#include <sstream>

LibraryData::LibraryData(const std::string& name, const std::string& description) : ReflectionData(name, "", description, nullptr, false, true) {}

void LibraryData::Copy(void* object, void* data) const
{
	//for (MemberMap::const_iterator i = Members.begin(); i != Members.end(); ++i)
	//	i->second->Copy(i->second->GetLocation(object), i->second->GetLocation(data));
}

std::string LibraryData::Format(void* object) const
{
	//JSONObject json;
	//
	//for (MemberMap::const_iterator i = members.begin(); i != members.end(); ++i)
	//	json.members[i->first] = i->second->format(object);
	//
	//return json.encode();

	return "";
}

void LibraryData::Parse(void* object, const std::string& data) const
{
	//JSONObject json(data);
	//
	//for (JSONObject::MemberMap::iterator i = json.members.begin(); i != json.members.end(); ++i)
	//{
	//	const Member* member = members.find(i->first)->second;
	//	
	//	if (member != nullptr)
	//		member->parse(object, i->second);
	//}
}

ClassData::ClassData(const std::string& name, const std::string& description, const std::string& inherits) : ReflectionData(name, inherits, description, GetType(inherits), true)
{
	LoadInheritedData();
}

ClassData::ClassData(const std::string& name, const std::string& description, const ReflectionData* inherits) : ReflectionData(name, inherits == nullptr ? "" : inherits->Name, description, inherits, true)
{
	LoadInheritedData();
}

void ClassData::Copy(void* object, void* data) const
{
	for (MemberMap::const_iterator i = Members.begin(); i != Members.end(); ++i)
		i->second->Copy(i->second->GetLocation(object), i->second->GetLocation(data));
}

std::string ClassData::Format(void* object) const
{
	//JSONObject json;
	//
	//for (MemberMap::const_iterator i = members.begin(); i != members.end(); ++i)
	//	json.members[i->first] = i->second->format(object);
	//
	//return json.encode();

	return "";
}

void ClassData::Parse(void* object, const std::string& data) const
{
	//JSONObject json(data);
	//
	//for (JSONObject::MemberMap::iterator i = json.members.begin(); i != json.members.end(); ++i)
	//{
	//	const Member* member = members.find(i->first)->second;
	//	
	//	if (member != nullptr)
	//		member->parse(object, i->second);
	//}
}

void ClassData::LoadInheritedData()
{
	if (ParentData == nullptr)
		return;

	for (MemberMap::const_iterator i = ParentData->Members.begin(); i != ParentData->Members.end(); ++i)
		AddMember(i->second->Name, ParentData);
		//AddMember(i->second->Name, i->second->Type, i->second->GetLocation(nullptr), i->second->Description, i->second->Archivable, i->second->IsStatic);

	for (FunctionMap::const_iterator i = ParentData->Functions.begin(); i != ParentData->Functions.end(); ++i)
		if (i->second->Name != i->second->ParentName)
			AddFunction(i->second->Name, ParentData);
}

ClassData::Property* ClassData::RegisterProperty(const std::string& name)
{
	PropertyMap::iterator property = Properties.find(name);
		
	if (property != Properties.end())
		throw "Attempt to register duplicate property '" + name + "'";
		
	Properties[name] = Property();
		
	property = Properties.find(name);
		
	property->second.Name = name;
	property->second.ParentType = this;

	PropertyList.push_back(name);
		
	return &property->second;
}

const ClassData::StringVector& ClassData::GetProperties() const
{
	return PropertyList;
}

const ClassData::Property* ClassData::GetProperty(const std::string& name) const
{
	PropertyMap::const_iterator property = Properties.find(name);

	if (property != Properties.end())
		return &property->second;

	return nullptr;
}

void ClassData::LoadProperties(const ClassData* parent)
{
	Properties = parent->Properties;
	PropertyList = parent->PropertyList;
}

void ClassData::Property::AddGetter(const std::string& name, const std::string& typeName)
{
	if (Getter != nullptr)
		throw "Attempt to add duplicate getter function '" + Name + "' of type '" + typeName + "' to object of type '" + ParentType->Name + "'.";

	const ReflectionData::Function* function = ParentType->GetFunction(name);

	if (function == nullptr)
		throw "Attempt to add undefined getter function '" + name + "' of type '" + typeName + "' to object of type '" + ParentType->Name + "'.";

	LuaFunctionOverload overload = nullptr;

	for (int i = 0; overload == nullptr && i < function->GetOverloads(); ++i)
	{
		LuaFunctionOverload currentOverload = function->GetOverload<int, lua_State*>(i);

		if (currentOverload->GetParameters() == 0 && currentOverload->ReturnTypeName == typeName)
			overload = currentOverload;
	}

	if (overload == nullptr)
		throw "Attempt to add invalid getter function '" + Name + "' of type '" + typeName + "' to object of type '" + ParentType->Name + "'. Expected overload with a return value of type '" + typeName + "' and 0 arguments.";

	Getter = overload;
}

void ClassData::Property::AddSetter(const std::string& name, const std::string& typeName)
{
	const ReflectionData::Function* function = ParentType->GetFunction(name);

	if (function == nullptr)
		throw "Attempt to add undefined setter function '" + name + "' to object of type '" + ParentType->Name + "'.";

	LuaFunctionOverload matchingOverload = nullptr;

	for (int i = 0; i < function->GetOverloads(); ++i)
	{
		const ReflectionData::FunctionOverloadData* overload = function->Overloads[i];

		if ((overload->ReturnType == nullptr || overload->ReturnType->Name == "void") && overload->GetParameters() == 1 && overload->Parameters[0].TypeName == typeName)
		{
			matchingOverload = function->GetOverload<int, lua_State*>(i);

			break;
		}
	}

	if (matchingOverload == nullptr)
		throw "Attempt to add invalid setter function '" + name + "' to object of type '" + ParentType->Name + "'. Expected overload with 1 parameter of type '" + typeName + ".";

	Setters.push_back(matchingOverload);

}

void VoidData::Initialize()
{
	VoidData* data = new VoidData();
}

void BooleanData::Copy(void* object, void* data) const
{
	*(bool*)object = *(bool*)data;
}

std::string BooleanData::Format(void* object) const
{
	return *(bool*)object ? "true" : "false";
}

void BooleanData::Parse(void* object, const std::string& data) const
{
	*(bool*)object = data == "true";
}

void BooleanData::Initialize()
{
	BooleanData* data = new BooleanData();
}

void CharacterData::Copy(void* object, void* data) const
{
	*(char*)object = *(char*)data;
}

std::string CharacterData::Format(void* object) const
{
	return std::string("") + *(char*)object;
}

void CharacterData::Parse(void* object, const std::string& data) const
{
	*(char*)object = *data.c_str();
}

void CharacterData::Initialize()
{
	CharacterData* data = new CharacterData();
}

void UnsignedCharacterData::Copy(void* object, void* data) const
{
	*(char*)object = *(char*)data;
}

std::string UnsignedCharacterData::Format(void* object) const
{
	return std::string("") + *(char*)object;
}

void UnsignedCharacterData::Parse(void* object, const std::string& data) const
{
	*(char*)object = *data.c_str();
}

void UnsignedCharacterData::Initialize()
{
	UnsignedCharacterData* data = new UnsignedCharacterData();
}

void ShortData::Copy(void* object, void* data) const
{
	*(short*)object = *(short*)data;
}

std::string ShortData::Format(void* object) const
{
	std::stringstream data;

	data << *(short*)object;

	return data.str();
}

void ShortData::Parse(void* object, const std::string& data) const
{
	*(short*)object = atoi(data.c_str());
}

void ShortData::Initialize()
{
	ShortData* data = new ShortData();
}

void UnsignedShortData::Copy(void* object, void* data) const
{
	*(short*)object = *(short*)data;
}

std::string UnsignedShortData::Format(void* object) const
{
	std::stringstream data;

	data << *(short*)object;

	return data.str();
}

void UnsignedShortData::Parse(void* object, const std::string& data) const
{
	*(short*)object = atoi(data.c_str());
}

void UnsignedShortData::Initialize()
{
	UnsignedShortData* data = new UnsignedShortData();
}

void IntegerData::Copy(void* object, void* data) const
{
	*(int*)object = *(int*)data;
}

std::string IntegerData::Format(void* object) const
{
	std::stringstream data;

	data << *(int*)object;

	return data.str();
}

void IntegerData::Parse(void* object, const std::string& data) const
{
	*(int*)object = atoi(data.c_str());
}

void IntegerData::Initialize()
{
	IntegerData* data = new IntegerData();
}

void UnsignedIntegerData::Copy(void* object, void* data) const
{
	*(int*)object = *(int*)data;
}

std::string UnsignedIntegerData::Format(void* object) const
{
	std::stringstream data;

	data << *(int*)object;

	return data.str();
}

void UnsignedIntegerData::Parse(void* object, const std::string& data) const
{
	*(int*)object = atoi(data.c_str());
}

void UnsignedIntegerData::Initialize()
{
	UnsignedIntegerData* data = new UnsignedIntegerData();
}

void LongIntegerData::Copy(void* object, void* data) const
{
	*(int*)object = *(int*)data;
}

std::string LongIntegerData::Format(void* object) const
{
	std::stringstream data;

	data << *(int*)object;

	return data.str();
}

void LongIntegerData::Parse(void* object, const std::string& data) const
{
	*(int*)object = atoi(data.c_str());
}

void LongIntegerData::Initialize()
{
	LongIntegerData* data = new LongIntegerData();
}

void UnsignedLongIntegerData::Copy(void* object, void* data) const
{
	*(int*)object = *(int*)data;
}

std::string UnsignedLongIntegerData::Format(void* object) const
{
	std::stringstream data;

	data << *(int*)object;

	return data.str();
}

void UnsignedLongIntegerData::Parse(void* object, const std::string& data) const
{
	*(int*)object = atoi(data.c_str());
}

void UnsignedLongIntegerData::Initialize()
{
	UnsignedLongIntegerData* data = new UnsignedLongIntegerData();
}

void LongLongIntegerData::Copy(void* object, void* data) const
{
	*(int*)object = *(int*)data;
}

std::string LongLongIntegerData::Format(void* object) const
{
	std::stringstream data;

	data << *(int*)object;

	return data.str();
}

void LongLongIntegerData::Parse(void* object, const std::string& data) const
{
	*(int*)object = atoi(data.c_str());
}

void LongLongIntegerData::Initialize()
{
	LongLongIntegerData* data = new LongLongIntegerData();
}

void UnsignedLongLongIntegerData::Copy(void* object, void* data) const
{
	*(int*)object = *(int*)data;
}

std::string UnsignedLongLongIntegerData::Format(void* object) const
{
	std::stringstream data;

	data << *(int*)object;

	return data.str();
}

void UnsignedLongLongIntegerData::Parse(void* object, const std::string& data) const
{
	*(int*)object = atoi(data.c_str());
}

void UnsignedLongLongIntegerData::Initialize()
{
	UnsignedLongLongIntegerData* data = new UnsignedLongLongIntegerData();
}

void FloatData::Copy(void* object, void* data) const
{
	*(float*)object = *(float*)data;
}

std::string FloatData::Format(void* object) const
{
	std::stringstream data;

	data << *(float*)object;

	return data.str();
}

void FloatData::Parse(void* object, const std::string& data) const
{
	*(float*)object = float(atof(data.c_str()));
}

void FloatData::Initialize()
{
	FloatData* data = new FloatData();
}

void DoubleData::Copy(void* object, void* data) const
{
	*(double*)object = *(double*)data;
}

std::string DoubleData::Format(void* object) const
{
	std::stringstream data;

	data << *(double*)object;

	return data.str();
}

void DoubleData::Parse(void* object, const std::string& data) const
{
	*(double*)object = atof(data.c_str());
}

void DoubleData::Initialize()
{
	DoubleData* data = new DoubleData();
}

void StringData::Copy(void* object, void* data) const
{
	*(std::string*)object = *(std::string*)data;
}

std::string StringData::Format(void* object) const
{
	return *(std::string*)object;
}

void StringData::Parse(void* object, const std::string& data) const
{
	*(std::string*)object = data;
}

void StringData::Initialize()
{
	StringData* data = new StringData();
}
