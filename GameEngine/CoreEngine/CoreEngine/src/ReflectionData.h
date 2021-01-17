#pragma once

#include <string>
#include <map>
#include <vector>
#include <functional>
#include <memory>

typedef void(*VoidFunction)(int);

namespace Engine
{
	class ObjectBase;
}

class ReflectionData
{
public:
	class Data
	{
	public:
		std::string Description;
		int ParentTypeID = -1;
		std::string Name;
		std::string ParentName;
		const ReflectionData* ParentType = nullptr;
		
		virtual ~Data() {}

	protected:
	};
	
	class Member : public Data
	{
	public:
		bool IsStatic = false;
		bool Archivable = false;
		bool ReadOnly = false;
		int TypeID = -1;
		std::string TypeName;
		const ReflectionData* Type = nullptr;

		virtual ~Member() {}
		
		void Copy(void* object, void* data) const;
		std::string Format(void* object) const;
		void Parse(void* object, const std::string& data) const;
		
		void* GetLocation(void* object) const;
		
		friend class ReflectionData;
	
	private:
		int Offset = 0;
	};

	struct FunctionOverloadData : public Data
	{
		struct ParameterData
		{
			std::string Description;
			std::string Name;
			std::string DefaultValue = "";
			std::string TypeName;
			const ReflectionData* Type = nullptr;
			bool HasDefaultValue = false;

			ParameterData(const std::string& description, const std::string& name, const std::string& defaultValue, const std::string& typeName, const ReflectionData* type, bool hasDefaultValue) : Description(description), Name(name), DefaultValue(defaultValue), TypeName(typeName), Type(type), HasDefaultValue(hasDefaultValue) {}
		};

		typedef std::vector<ParameterData> ParameterVector;

		bool IsStatic = false;
		std::string ReturnValueDescription;
		std::string ReturnTypeName = "void";
		const ReflectionData* ReturnType = nullptr;
		ParameterVector Parameters;

		virtual ~FunctionOverloadData() {}

		int GetParameters() const { return int(Parameters.size()); }

		template<typename T>
		const T* Cast()
		{
			return reinterpret_cast<T*>(this);
		}
	};

	template <typename Return, typename... Args>
	struct FunctionOverload : public FunctionOverloadData
	{
		typedef std::function<Return(Args...)> FunctionCallback;

		FunctionCallback Callback;

		virtual ~FunctionOverload() {}
	};

	struct Function : public Data
	{
		typedef std::vector<FunctionOverloadData*> OverloadVector;

		virtual ~Function()
		{
			for (int i = 0; i < GetOverloads(); ++i)
				delete Overloads[i];
		}

		OverloadVector Overloads;

		int GetOverloads() const { return int(Overloads.size()); }

		template <typename Return, typename... Args>
		FunctionOverload<Return, Args...>* GetOverload(int overload)
		{
			return reinterpret_cast<FunctionOverload<Return, Args...>*>(Overloads[overload]);
		}

		template <typename Return, typename... Args>
		const FunctionOverload<Return, Args...>* GetOverload(int overload) const
		{
			return reinterpret_cast<FunctionOverload<Return, Args...>*>(Overloads[overload]);
		}

		template <typename Return, typename... Args>
		const typename FunctionOverload<Return, Args...>::FunctionCallback& GetCallback(int callback) const
		{
			return reinterpret_cast<FunctionOverload<Return, Args...>*>(Overloads[callback])->Callback;
		}
	};
	
	typedef std::map<std::string, ReflectionData*> DataMap;
	typedef std::vector<ReflectionData*> DataVector;
	typedef std::map<std::string, const Member*> MemberMap;
	typedef std::map<std::string, int> IDMap;
	typedef std::vector<std::string> NameVector;
	typedef FunctionOverloadData::ParameterVector ParameterVector;
	typedef Function::OverloadVector OverloadVector;
	typedef std::map<std::string, const Function*> FunctionMap;
	typedef std::function<std::shared_ptr<Engine::ObjectBase>()> AllocateCallback;
	typedef std::function<void*()> AllocateRawCallback;
	typedef std::function<void(void* output)> DeleteCallback;
	typedef std::function<void(void* object, void* copying)> CopyCallback;
	
	int ID = 0;
	bool IsClass = true;
	bool IsLibrary = false;
	bool IsEnum = false;
	bool UseReferences = true;
	std::string Name;
	std::string Description;
	int ParentID;
	std::string ParentName;
	const ReflectionData* ParentData;
	MemberMap Members;
	FunctionMap Functions;
	AllocateCallback Create;
	AllocateRawCallback CreateRaw;
	DeleteCallback Delete;
	CopyCallback CopyConstructor;
	CopyCallback AssignmentOperator;

	const Member* GetMember(const std::string& member) const;
	const Function* GetFunction(const std::string& function) const;
	
	bool Inherits(const std::string& typeName) const;
	
	virtual void Copy(void* object, void* data) const = 0;
	virtual std::string Format(void* object) const = 0;
	virtual void Parse(void* object, const std::string& data) const = 0;
	
	static int GetTypeID(const std::string& name);
	static std::string GetTypeName(int id);
	static const DataVector& GetTypes();
	static const ReflectionData* GetType(const std::string& name);
	static const ReflectionData* GetType(int typeID);
	static void CleanUp();
	static std::ostream& Dump(std::ostream& out);
	
protected:
	ReflectionData(const std::string& name, const std::string& parentName = std::string(), const std::string& description = std::string(), const ReflectionData* parentData = nullptr, bool isClass = false, bool isLibrary = false);
	virtual ~ReflectionData();
	
	void AddMember(const std::string& memberName, const std::string& typeName, void* data, const std::string& description = std::string(), bool archivable = true, bool isStatic = false, bool readOnly = false);
	void AddMember(const std::string& memberName, const ReflectionData* type, void* data, const std::string& description = std::string(), bool archivable = true, bool isStatic = false, bool readOnly = false);
	void AddMember(const std::string& memberName, const ReflectionData* parent);
	void AddFunction(const std::string& functionName, Function* data);
	void AddFunction(const std::string& functionName, const ReflectionData* parent);

private:
	static DataVector Types;
	static IDMap IDs;
	static NameVector Names;
};

void initializeCoreTypes();

typedef const ReflectionData MetaData;
