#pragma once

#include <memory>

#include "IdentifierHeap.h"
#include "ObjectReflection.h"
#include "LuaStackTypes.h"
#include "ObjectAllocator.h"
#include "ObjectLuaType.h"
#include <iostream>

namespace Engine
{
	class ObjectBase
	{
	public:
		typedef std::function<std::shared_ptr<ObjectBase>()> FactoryCallback;
		typedef std::vector<std::string> StringVector;

		std::string Name = "ObjectBase";
		std::weak_ptr<ObjectBase> This;

		virtual ~ObjectBase();

		void Initialize();

		int GetTypeID() const;
		const std::string& GetTypeName() const;
		bool IsA(const std::string& className, bool inherited = true) const;
		bool IsA(MetaData* type, bool inherited = true) const;
		const ClassData::Property* GetProperty(const std::string& name) const;

		virtual void Update(float delta) {}

		bool HasRequirements() const;

		void SetObjectID(int id);
		int GetObjectID() const;

		template <typename T>
		bool IsA(bool inherited = true);

		template <typename T>
		std::shared_ptr<T> Cast();

		template <typename T>
		std::shared_ptr<T> Cast() const;

		operator std::string() const;

		static const FactoryCallback& GetFactoryFunction(const std::string& className);

		template <typename T = ObjectBase>
		static std::shared_ptr<T> GetObjectFromID(int id);

	protected:
		static void RegisterFactoryFunction(const std::string& typeName, const FactoryCallback& factory)
		{
			FactoryFunctions[typeName] = factory; 
		}

	private:
		typedef std::map<std::string, FactoryCallback> FactoryCallbackMap;
		typedef IDHeap<std::weak_ptr<ObjectBase>> ObjectHandleHeap;

		static ObjectHandleHeap ObjectIDs;
		static FactoryCallbackMap FactoryFunctions;

		int ObjectID = -1;

		NonInstantiable;

		Base_Class;

		Reflected(ObjectBase);
	};

	////Class_Inherits(ObjectBase, CoreClass);

	template <typename T>
	std::shared_ptr<T> ObjectBase::Cast()
	{
		//if (this == nullptr)
		//	return nullptr;

		return std::dynamic_pointer_cast<T>(This.lock());
	}

	template <typename T>
	std::shared_ptr<T> ObjectBase::Cast() const
	{
		return std::dynamic_pointer_cast<T>(This.lock());
	}


	template <typename T>
	std::shared_ptr<T> Create()
	{
		return T::ObjectData::Create()->Cast<T>();
	}

	std::shared_ptr<ObjectBase> CreateObject(const std::string& className);

	class Object : public ObjectBase
	{
	public:
		virtual ~Object();

		bool SiblingComponents = false;
		bool ParentComponent = true;
		bool AncestorComponents = true;
		bool SuperSiblingComponents = false;
		int SuperComponentHeight = -1;

		void Initialize();
		void Update(float delta);

		std::string GetFullName() const;
		int GetChildren() const;
		std::shared_ptr<ObjectBase> Get(const std::string& className, bool inherited = true);
		std::shared_ptr<ObjectBase> Get(int index);
		std::shared_ptr<ObjectBase> GetByName(const std::string& name);
		std::shared_ptr<ObjectBase> GetAncestor(const std::string& className, bool inherited = true);
		bool HasA(const std::string& className, bool inherited = true) const;

		void AddChild(const std::shared_ptr<ObjectBase>& child);
		void RemoveChild(const std::shared_ptr<ObjectBase>& child);
		void Remove();
		void SetParent(const std::shared_ptr<ObjectBase>& newParent);
		std::shared_ptr<ObjectBase> GetParent() const;
		std::shared_ptr<ObjectBase> GetComponent(const std::string& className, bool inherited = true) const;
		bool CheckRestriction(const std::shared_ptr<ObjectBase>& object) const;
		const std::string& CheckRequirements(const std::shared_ptr<ObjectBase>& object) const;
		bool IsAncestorOf(const std::shared_ptr<ObjectBase>& object) const;
		bool IsDescendantOf(const std::shared_ptr<ObjectBase>& object) const;

		template <typename T>
		std::shared_ptr<T> Get(bool inherited = true);

		template <typename T>
		std::shared_ptr<T> Get(int index);

		template <typename T>
		std::shared_ptr<T> GetByName(const std::string& name);

		template <typename T>
		std::shared_ptr<T> GetAncestor(bool inherited = true);

		template <typename T>
		std::shared_ptr<T> GetComponent(bool inherited = true) const;

		template <typename T>
		bool HasA(bool inherited = true);

	private:
		typedef std::vector<std::shared_ptr<ObjectBase>> ObjectVector;

		std::weak_ptr<ObjectBase> Parent;
		ObjectVector Children;

		std::shared_ptr<ObjectBase> GetComponent(MetaData* data, bool inherited) const;

		Instantiable;

		Inherits_Class(ObjectBase);

		Reflected(Object);
	};

	////Class_Inherits(Object, ObjectBase);

	template <typename T>
	std::shared_ptr<T> Object::Get(bool inherited)
	{
		for (int i = 0; i < GetChildren(); ++i)
			if (Get(i)->GetMetaData() == T::GetClassMetaData() || (inherited && Get(i)->GetMetaData()->Inherits(T::GetClassMetaData()->Name)))
				return Get(i)->Cast<T>();

		return nullptr;
	}

	template <typename T>
	std::shared_ptr<T> Object::Get(int index)
	{
		return Get(index)->Cast<T>();
	}

	template <typename T>
	std::shared_ptr<T> Object::GetByName(const std::string& name)
	{
		return GetByName(name)->Cast<T>();
	}

	template <typename T>
	std::shared_ptr<T> Object::GetAncestor(bool inherited)
	{
		std::shared_ptr<Object> ancestor = Parent.lock();

		while (ancestor != nullptr)
		{
			if (ancestor->GetMetaData() == T::GetClassMetaData() || (inherited && ancestor->GetMetaData()->Inherits(T::GetClassMetaData()->Name)))
				return ancestor;

			ancestor = ancestor->Parent;
		}

		return nullptr;
	}

	template <typename T>
	std::shared_ptr<T> Object::GetComponent(bool inherited) const
	{
		std::shared_ptr<ObjectBase> child = GetComponent(T::GetClassMetaData(), inherited);

		if (child == nullptr)
			return nullptr;

		return child->Cast<T>();
	}

	template <typename T>
	bool Object::HasA(bool inherited)
	{
		return HasA(T::GetClassMetaData()->Name, inherited);
	}

	template <typename T>
	bool ObjectBase::IsA(bool inherited)
	{
		return IsA(T::GetClassMetaData()->Name, inherited);
	}

	template <typename T>
	std::shared_ptr<T> ObjectBase::GetObjectFromID(int id)
	{
		if (id != -1)
			return ObjectIDs.GetNode(id).GetData().lock()->Cast<T>();

		return nullptr;
	}

	//template <typename T>
	//std::shared_ptr<T> ObjectBase::GetObjectFromID<std::shared_ptr<T>>(int id)
	//{
	//	return ObjectIDs.GetNode(id).GetData().lock()->Cast<T>();
	//}
	//
	//template <typename T>
	//std::shared_ptr<T> ObjectBase::GetObjectFromID<std::weak_ptr<T>>(int id)
	//{
	//	return ObjectIDs.GetNode(id).GetData().lock()->Cast<T>();
	//}
}
