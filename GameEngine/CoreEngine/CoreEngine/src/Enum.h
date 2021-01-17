#pragma once

#include <map>
#include <string>
#include <ostream>

#include "CoreTypeReflection.h"

namespace Engine
{
	class EnumClass;

	class EnumType
	{
	public:
		class EnumItem
		{
		public:
			EnumItem(const std::string& name = "", const std::string& description = "", int value = 0) : Name(name), Description(description), Value(value) {}

			std::string Description;
			std::string Name;
			int Value = 0;
		};

		typedef std::map<int, EnumItem> EnumItemMap;
		typedef std::map<std::string, int> EnumItemNameMap;
		typedef std::map<std::string, EnumType> EnumTypeMap;

		std::string Description;
		std::string Name;
		EnumItemMap Items;
		EnumItemNameMap ItemNames;

		EnumType(const std::string& name = "", const std::string& description = "") : Name(name), Description(description) {}

		void AddItem(const std::string& name, int value, const std::string& description = "");

		void InitReflection();

		static EnumType& AddType(const std::string& name);
		static const EnumType& GetType(const std::string& name);
		static const EnumTypeMap& GetTypes();
		static std::ostream& Dump(std::ostream& out);

	private:
		static EnumTypeMap Types;
		static EnumType Nope;

		EnumClass* Meta = nullptr;
	};

	class EnumClass : public ClassData
	{
	public:
		static EnumClass* Type;

		std::string EnumName;
		Function* CleanMe = nullptr;

		EnumClass(const std::string& name, const std::string& description, EnumClass* parent = Type) : ClassData(name, description, parent) {}
		virtual ~EnumClass();

		void Initialize(const std::string& name, const int& value);
		void AddMethod(const std::string& name, ReflectionData::Function* function);
	};
}
