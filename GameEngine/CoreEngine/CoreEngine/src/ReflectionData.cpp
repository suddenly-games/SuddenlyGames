#include <sstream>
extern "C" {
#include <stdlib.h>
}
#include <iostream>
#include <set>

#include "ReflectionData.h"
#include "CoreTypeReflection.h"
//#include "JSON.h"

ReflectionData::DataVector ReflectionData::Types;
ReflectionData::IDMap ReflectionData::IDs;
ReflectionData::NameVector ReflectionData::Names;

void ReflectionData::Member::Copy(void* object, void* data) const
{
	Type->Copy(object, data);
}

std::string ReflectionData::Member::Format(void* object) const
{
	return Type->Format(GetLocation(object));
}

void ReflectionData::Member::Parse(void* object, const std::string& data) const
{
	Type->Parse(GetLocation(object), data);
}

void* ReflectionData::Member::GetLocation(void* object) const
{
	char* location = IsStatic ? nullptr : (char*)object;
	
	return (void*)(location + Offset);
}

const ReflectionData::Member* ReflectionData::GetMember(const std::string& member) const
{
	MemberMap::const_iterator i = Members.find(member);

	if (i != Members.end())
		return i->second;

	return nullptr;
}

const ReflectionData::Function* ReflectionData::GetFunction(const std::string& function) const
{
	FunctionMap::const_iterator i = Functions.find(function);

	if (i != Functions.end())
		return i->second;

	return nullptr;
}

bool ReflectionData::Inherits(const std::string& typeName) const
{
	if (Name == typeName)
		return true;
	else if (ParentID == -1)
		return false;
	else
		return GetType(ParentID)->Inherits(typeName);
}

ReflectionData::ReflectionData(const std::string& name, const std::string& parentName, const std::string& description, const ReflectionData* parentData, bool isClass, bool isLibrary) : ID(int(Types.size())), Name(name), Description(description), ParentID(GetTypeID(parentName)), ParentName(parentName), ParentData(parentData), IsClass(isClass), IsLibrary(isLibrary)
{
	if (GetType(name) != nullptr)
		throw std::string("type '") + name + "' already registered";
	
	IDs[name] = ID;
	Types.push_back(this);
	Names.push_back(name);

}

ReflectionData::~ReflectionData()
{
	for (MemberMap::iterator i = Members.begin(); i != Members.end(); ++i)
		if (i->second->ParentType == this)
			delete i->second;

	for (FunctionMap::iterator i = Functions.begin(); i != Functions.end(); ++i)
		if (i->second->ParentType == this)
			delete i->second;
}

void ReflectionData::AddMember(const std::string& memberName, const std::string& typeName, void* data, const std::string& description, bool archivable, bool isStatic, bool readOnly)
{
	const ReflectionData* type = GetType(typeName);
	
	if (type == nullptr)
		throw std::string("failed to add member '") + memberName + "' of type '" + typeName + "' to '" + Name + "'";
	
	AddMember(memberName, type, data, description, archivable, isStatic, readOnly);
}

void ReflectionData::AddMember(const std::string& memberName, const ReflectionData* type, void* data, const std::string& description, bool archivable, bool isStatic, bool readOnly)
{
	if (Members.find(memberName) != Members.end())
		throw std::string("type '") + Name + "' already has member '" + memberName + "' of type '" + Members[memberName]->Name + "'";
	
	Member* member = new Member();
	
	member->ParentTypeID = ID;
	member->Name = memberName;
	member->ParentName = Name;
	member->ParentType = this;
	
	member->ReadOnly = readOnly;
	member->IsStatic = IsLibrary || isStatic;
	member->Archivable = archivable;
	member->Offset = int((char*)data - (char*)nullptr);
	member->TypeID = type->ID;
	member->TypeName = type->Name;
	member->Type = type;
	
	Members[memberName] = member;
}

void ReflectionData::AddMember(const std::string& memberName, const ReflectionData* parent)
{
	if (parent->Members.find(memberName) == parent->Members.end())
		throw std::string("inherited type '") + parent->Name + "' doesn't have a member called '" + memberName + "' to inherit";

	Members[memberName] = parent->Members.find(memberName)->second;
}

void ReflectionData::AddFunction(const std::string& functionName, Function* data)
{
	if (Functions.find(functionName) != Functions.end())
	{
		//throw std::string("type '") + Name + "' already has function '" + functionName;
		Functions.erase(functionName);
	}

	for (int i = 0; i < data->GetOverloads(); ++i)
	{
		data->Overloads[i]->ReturnType = GetType(data->Overloads[i]->ReturnTypeName);

		if (data->Overloads[i]->ReturnType == nullptr)
			throw std::string("function overload of '") + functionName + "' in type '" + Name + "' has an unregistered return value type of '" + data->Overloads[i]->ReturnTypeName + "'";

		for (int j = 0; j < data->Overloads[i]->GetParameters(); ++j)
		{
			data->Overloads[i]->Parameters[j].Type = GetType(data->Overloads[i]->Parameters[j].TypeName);

			if (data->Overloads[i]->Parameters[j].Type == nullptr)
				throw std::string("parameter '") + data->Overloads[i]->Parameters[j].Name + "' in function overload of '" + functionName + "' in type '" + Name + "' has an unregistered type of '" + data->Overloads[i]->Parameters[j].TypeName + "'";
		}
	}

	Functions[functionName] = data;

	data->ParentTypeID = ID;
	data->Name = functionName;
	data->ParentName = Name;
	data->ParentType = this;
}

void ReflectionData::AddFunction(const std::string& functionName, const ReflectionData* parent)
{
	if (parent->Functions.find(functionName) == parent->Functions.end())
		throw std::string("inherited type '") + parent->Name + "' doesn't have a function called '" + functionName + "' to inherit";

	Functions[functionName] = parent->Functions.find(functionName)->second;
}

int ReflectionData::GetTypeID(const std::string& name)
{
	if (IDs.find(name) != IDs.end())
		return IDs[name];
	
	return -1;
}

std::string ReflectionData::GetTypeName(int id)
{
	if (id >= 0 && id < int(Names.size()))
		return Names[id];
	
	return "";
}

const ReflectionData::DataVector& ReflectionData::GetTypes()
{
	return Types;
}

const ReflectionData* ReflectionData::GetType(const std::string& name)
{
	int typeID = GetTypeID(name);
	
	return GetType(typeID);
}

const ReflectionData* ReflectionData::GetType(int typeID)
{
	if (typeID != -1)
		return Types[typeID];
	
	return nullptr;
}

void ReflectionData::CleanUp()
{
	typedef std::set<const Member*> MemberSet;
	typedef std::set<const Function*> FunctionSet;

	MemberSet members;
	FunctionSet functions;

	for (int i = 0; i < int(Types.size()); ++i)
	{
		for (MemberMap::iterator j = Types[i]->Members.begin(); j != Types[i]->Members.end(); ++j)
			members.insert(j->second);

		for (FunctionMap::iterator j = Types[i]->Functions.begin(); j != Types[i]->Functions.end(); ++j)
			functions.insert(j->second);

		Types[i]->Members.clear();
		Types[i]->Functions.clear();

		delete Types[i];
	}

	for (MemberSet::iterator i = members.begin(); i != members.end(); ++i)
		delete *i;

	Types.clear();
	IDs.clear();
	Names.clear();
}

namespace
{
	typedef std::set<std::string> StringSet;
	typedef std::map<std::string, StringSet> StringMap;
	
	std::ostream& printTabs(std::ostream& out, int tabs)
	{
		for (int i = 0; i < tabs; ++i)
			out << '\t';
		
		return out;
	}
	
	void dumpOverload(std::ostream& out, const ReflectionData::FunctionOverloadData* overload, int tabs = 0)
	{
		printTabs(out, tabs);

		if (overload->IsStatic)
			out << "static ";

		if (overload->Name != overload->ParentName)
			out << overload->ReturnTypeName << ' ';

		out << overload->Name << "( ";
		
		for (int i = 0; i < overload->GetParameters(); ++i)
			std::cout << overload->Parameters[i].TypeName << " " << overload->Parameters[i].Name << (overload->Parameters[i].HasDefaultValue ? " = " + overload->Parameters[i].DefaultValue : "") << (i < overload->GetParameters() - 1 ? ", " : " ");
		
		std::cout << ")" << std::endl;	
	}
	
	void dumpFunction(std::ostream& out, const ReflectionData* type, const ReflectionData::Function* function, int tabs)
	{
		if (function->ParentType == type)
		{
			for (int i = 0; i < function->GetOverloads(); ++i)
				dumpOverload(out, function->Overloads[i], tabs);
		}
	}
	
	void dumpData(const ReflectionData* type, StringMap& tree, std::ostream& out, int tabs = 0)
	{
		out << std::endl;
		
		printTabs(out, tabs) << (type->IsClass ? "Class " : (type->IsLibrary ? "Library " : (type->IsEnum ? "Enum " : "Type "))) << type->Name << std::endl;
		
		if (type->Members.size() > 0)
			printTabs(out, tabs + 1) << (type->IsClass || type->IsEnum ? "Properties:" : "Entries:") << std::endl;
		
		for (ReflectionData::MemberMap::const_iterator i = type->Members.begin(); i != type->Members.end(); ++i)
			if (i->second->ParentType == type)
				printTabs(out, tabs + 2) << (i->second->IsStatic ? "static " : "") << i->second->TypeName << ' ' << i->first << std::endl;

		if (type->Functions.size() > 0)
			printTabs(out, tabs + 1) << (type->IsClass ? "Methods:" : "Functions:") << std::endl;

		for (ReflectionData::FunctionMap::const_iterator i = type->Functions.begin(); i != type->Functions.end(); ++i)
			if (i->second->ParentType == type)
				dumpFunction(out, type, i->second, tabs + 2);
		
		if (tree.find(type->Name) != tree.end())
		{
			StringSet& children = tree[type->Name];
			
			for (StringSet::iterator i = children.begin(); i != children.end(); ++i)
				dumpData(ReflectionData::GetType(*i), tree, out, tabs + 1);
		}
	}
}

std::ostream& ReflectionData::Dump(std::ostream& out)
{
	StringMap inheritanceTree;
	
	for (DataVector::iterator i = Types.begin(); i != Types.end(); ++i)
	{
		const ReflectionData* type = *i;
		
		if (type->ParentName == "")
			inheritanceTree[type->Name];
		else
			inheritanceTree[type->ParentName].insert(type->Name);
	}
	
	for (StringMap::iterator i = inheritanceTree.begin(); i != inheritanceTree.end(); ++i)
	{
		const ReflectionData* type = Types[GetTypeID(i->first)];
		
		if (type->ParentName == "")
			dumpData(type, inheritanceTree, out);
	}

	return out;
}

void initializeCoreTypes()
{
	VoidData::Initialize();
	BooleanData::Initialize();
	CharacterData::Initialize();
	UnsignedCharacterData::Initialize();
	ShortData::Initialize();
	UnsignedShortData::Initialize();
	IntegerData::Initialize();
	UnsignedIntegerData::Initialize();
	LongIntegerData::Initialize();
	UnsignedLongIntegerData::Initialize();
	LongLongIntegerData::Initialize();
	UnsignedLongLongIntegerData::Initialize();
	FloatData::Initialize();
	DoubleData::Initialize();
	StringData::Initialize();
}
