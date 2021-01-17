#include "Enum.h"

#include <iomanip>

#include "Lua.h"

namespace Engine
{
	EnumType::EnumTypeMap EnumType::Types = EnumTypeMap();
	EnumType EnumType::Nope = EnumType{ "Doesn't exist" };

	void EnumType::AddItem(const std::string& name, int value, const std::string& description)
	{
		Items[value] = EnumItem(name, description, value);
		ItemNames[name] = value;
	}

	EnumType& EnumType::AddType(const std::string& name)
	{
		Types[name] = EnumType(name, "");

		return Types[name];
	}

	const EnumType& EnumType::GetType(const std::string& name)
	{
		EnumTypeMap::iterator type = Types.find(name);

		if (type != Types.end())
			return type->second;

		return Nope;
	}

	const EnumType::EnumTypeMap& EnumType::GetTypes()
	{
		return Types;
	}

	std::ostream& EnumType::Dump(std::ostream& out)
	{
		out << "Enum {" << std::endl;

		for (EnumTypeMap::iterator i = Types.begin(); i != Types.end(); ++i)
		{
			out << "\t" << i->first << " {" << std::endl;

			for (EnumItemMap::iterator j = i->second.Items.begin(); j != i->second.Items.end(); ++j)
				out << "\t\t" << std::setw(4) << j->second.Value << ": " << j->second.Name << ";" << std::endl;

			out << "\t}" << std::endl;
		}

		out << "}" << std::endl;

		return out;
	}

	void EnumType::InitReflection()
	{
		EnumClass* type = new EnumClass(Name, Description);

		type->IsClass = false;
		type->IsEnum = true;

		for (EnumType::EnumItemMap::const_iterator j = Items.begin(); j != Items.end(); ++j)
		{
			EnumClass* item = new EnumClass("Enum." + Name + "." + j->second.Name, j->second.Description, type);

			item->EnumName = j->second.Name;
			item->Initialize(j->second.Name, j->second.Value);
			item->IsClass = false;
			item->IsEnum = true;

			EnumClass::Function* method = new EnumClass::Function();

			method->Name = "operator string";
			method->ParentName = item->Name;
			method->ParentType = item;
			method->ParentTypeID = item->ID;

			EnumClass::FunctionOverload<int, lua_State*>* overload = new EnumClass::FunctionOverload<int, lua_State*>();

			overload->Name = "operator string";
			overload->ReturnTypeName = "string";

			overload->Callback = [item] (lua_State* lua) -> int
			{
				lua_pushstring(lua, item->EnumName.c_str());

				return 1;
			};

			method->Overloads.push_back(overload);

			item->AddMethod("operator string", method);
			item->CleanMe = method;
		}
	}

	EnumClass* EnumClass::Type = nullptr;

	EnumClass::~EnumClass() {
		if (CleanMe != nullptr)
			delete CleanMe;
	}

	void EnumClass::Initialize(const std::string& name, const int& value)
	{
		AddMember("Name", "string", (void*)&name, "The enum value's name.", false, true);
		AddMember("Value", "int", (void*)&value, "The enum value's value.", false, true);
	}

	void EnumClass::AddMethod(const std::string& name, ReflectionData::Function* function)
	{
		AddFunction(name, function);
	}
}
