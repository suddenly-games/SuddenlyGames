#include "Object.h"

#include <iostream>

#include "LuaBinding.h"

namespace Engine
{
	ObjectBase::FactoryCallbackMap ObjectBase::FactoryFunctions;
	ObjectBase::ObjectHandleHeap ObjectBase::ObjectIDs = ObjectHandleHeap();

	ObjectBase::~ObjectBase()
	{
		ObjectIDs.Release(ObjectID);
	}

	void ObjectBase::Initialize()
	{
		ObjectID = ObjectIDs.RequestID(This);
	}

	int ObjectBase::GetTypeID() const
	{
		return GetMetaData()->ID;
	}

	const std::string& ObjectBase::GetTypeName() const
	{
		return GetMetaData()->Name;
	}

	bool ObjectBase::IsA(const std::string& className, bool inherited) const
	{
		MetaData* data = GetMetaData();
		
		return data->Name == className || (inherited && data->Inherits(className));
	}

	bool ObjectBase::IsA(MetaData* metadata, bool inherited) const
	{
		MetaData* data = GetMetaData();

		return data == metadata || data->Inherits(metadata->Name);
	}

	const ClassData::Property* ObjectBase::GetProperty(const std::string& name) const
	{
		return reinterpret_cast<const ClassData*>(GetMetaData())->GetProperty(name);
	}

	bool ObjectBase::HasRequirements() const
	{
		return GetRequirements().size() > 0;
	}

	void ObjectBase::SetObjectID(int id)
	{
		if (ObjectID != -1)
			throw "Attempt to set ID";

		ObjectID = id;
	}

	int ObjectBase::GetObjectID() const
	{
		return ObjectID;
	}

	ObjectBase::operator std::string() const
	{
		return Name;
	}

	const ObjectBase::FactoryCallback& ObjectBase::GetFactoryFunction(const std::string& className)
	{
		FactoryCallbackMap::iterator i = FactoryFunctions.find(className);

		if (i != FactoryFunctions.end())
			return i->second;

		throw "Attempt to get factory function of unregistered type: " + className;
	}

	std::shared_ptr<ObjectBase> CreateObject(const std::string& className)
	{
		return ObjectBase::GetFactoryFunction(className)();
	}

	void Object::Initialize()
	{
		//SetObjectID(This.GetID());

		//std::cout << "Object(" << GetObjectID() << ")" << std::endl;
	}

	void Object::Update(float delta)
	{
		for (int i = 0; i < GetChildren(); ++i)
		{
			if (Children[i] != nullptr)
				Children[i]->UpdateBase(delta);
			else
				throw "bad child detected";
		}
	}

	Object::~Object()
	{
		//std::cout << "~Object(" << GetObjectID() << ")" << std::endl;
		auto parent = Parent.lock();

		if (parent != nullptr)
			parent->Cast<Object>()->RemoveChild(This.lock());

		while (GetChildren() > 0)
		{
			if (Children.back() != nullptr)
				Children.back()->Cast<Object>()->Parent.reset();

			Children.pop_back();
		}
	}

	std::string Object::GetFullName() const
	{
		if (Parent.expired())
			return Name;

		std::string name = Name;
		
		for (std::shared_ptr<Object> parent = Parent.lock()->Cast<Object>(); parent != nullptr && !parent->Parent.expired(); parent = parent->Parent.lock()->Cast<Object>())
			name = parent->Name + "." + name;

		return name;
	}

	int Object::GetChildren() const
	{
		return int(Children.size());
	}

	std::shared_ptr<ObjectBase> Object::Get(const std::string& className, bool inherited)
	{
		for (int i = 0; i < int(Children.size()); ++i)
			if (Children[i]->IsA(className, inherited))
				return Children[i];

		return nullptr;
	}

	std::shared_ptr<ObjectBase> Object::Get(int index)
	{
		if (index < 0 || index >= int(Children.size()))
			return nullptr;

		return Children[index];
	}

	std::shared_ptr<ObjectBase> Object::GetByName(const std::string& name)
	{
		for (int i = 0; i < int(Children.size()); ++i)
			if (Children[i]->Name == name)
				return Children[i];

		return nullptr;
	}

	std::shared_ptr<ObjectBase> Object::GetAncestor(const std::string& className, bool inherited)
	{
		auto ancestor = Parent.lock();

		while (ancestor != nullptr)
		{
			if (ancestor->IsA(className, inherited))
				return ancestor;

			ancestor = ancestor->Cast<Object>()->Parent.lock();
		}

		return nullptr;
	}

	std::shared_ptr<ObjectBase> Object::GetComponent(const std::string& className, bool inherited) const
	{
		return GetComponent(ReflectionData::GetType(className), inherited);
	}

	std::shared_ptr<ObjectBase> Object::GetComponent(MetaData* data, bool inherited) const
	{
		auto parent = Parent.lock();

		if (parent != nullptr)
		{
			if (ParentComponent && parent->IsA(data, inherited))
				return parent;

			if (SiblingComponents)
			{
				for (int i = 0; i < parent->Cast<Object>()->GetChildren(); ++i)
				{
					std::shared_ptr<Object> child = parent->Cast<Object>()->Get(i)->Cast<Object>();

					if (child != This.lock()->Cast<Object>() && child->IsA(data, inherited))
						return parent->Cast<Object>()->Get(i);
				}
			}

			if (AncestorComponents || SuperSiblingComponents)
			{
				std::shared_ptr<Object> ancestor = Parent.lock()->Cast<Object>();

				for (int height = 0; (SuperComponentHeight == -1 || height < SuperComponentHeight) && ancestor != nullptr; ++height)
				{
					if (AncestorComponents && ancestor->IsA(data, inherited))
						return ancestor;

					if (SuperSiblingComponents)
					{
						for (int i = 0; i < ancestor->GetChildren(); ++i)
						{
							std::shared_ptr<Object> child = ancestor->Get(i)->Cast<Object>();

							if (child != This.lock()->Cast<Object>() && child->IsA(data, inherited))
								return ancestor->Get(i);
						}
					}

					if (!ancestor->Parent.expired())
						ancestor = ancestor->Parent.lock()->Cast<Object>();
					else
						ancestor = nullptr;
				}
			}
		}

		return nullptr;
	}

	bool Object::HasA(const std::string& className, bool inherited) const
	{
		if (IsA(className))
			return true;

		for (int i = 0; i < int(Children.size()); ++i)
			if (Children[i]->IsA(className, inherited))
				return true;

		return false;
	}

	void Object::AddChild(const std::shared_ptr<ObjectBase>& child)
	{
		if (!child->Cast<Object>()->CheckRestriction(This.lock()))
			throw "Attempt to add duplicate child type: " + child->GetTypeName() + " restricts types of siblings; Adding \"" + child->Name + "\" to \"" + Name + "\"";

		const std::string& missingRequirement = child->Cast<Object>()->CheckRequirements(This.lock());

		if (missingRequirement != "")
			throw "Attempt to add child to object with missing required siblings: " + child->GetTypeName() + " requires a sibling of type " + missingRequirement + "; Adding \"" + child->Name + "\" to \"" + Name + "\"";

		child->Cast<Object>()->Parent = This;

		Children.push_back(child);
	}

	void Object::Remove()
	{
		SetParent(nullptr);
	}

	void Object::RemoveChild(const std::shared_ptr<ObjectBase>& child)
	{
		int index = 0;

		while (index < int(Children.size()) && Children[index] != child)
			++index;

		if (index < int(Children.size()))
		{
			if (index < int(Children.size()) - 1)
				Children[index] = Children[Children.size() - 1];

			Children.pop_back();
		}
	}

	void Object::SetParent(const std::shared_ptr<ObjectBase>& newParent)
	{
		if (IsAncestorOf(newParent))
			throw "Attempt to create circular reference: " + GetTypeName() + " '" + Name + "' is already an ancestor of " + newParent->GetTypeName() + " '" + newParent->Name + "'";

		auto parent = Parent.lock();

		if (parent != nullptr)
			parent->Cast<Object>()->RemoveChild(This.lock());

		if (newParent != nullptr)
			newParent->Cast<Object>()->AddChild(This.lock());
		else
			Parent.reset();
	}

	std::shared_ptr<ObjectBase> Object::GetParent() const
	{
		return Parent.lock();
	}

	bool Object::CheckRestriction(const std::shared_ptr<ObjectBase>& object) const
	{
		return !RestrictsSiblings() || object->Cast<Object>()->HasA(GetTypeName());
	}

	std::string NoRequirements = "";

	const std::string& Object::CheckRequirements(const std::shared_ptr<ObjectBase>& object) const
	{
		if (!HasRequirements())
			return NoRequirements;

		const StringVector& requirements = GetRequirements();

		for (int i = 0; i < int(requirements.size()); ++i)
			if (!object->Cast<Object>()->HasA(requirements[i]))
				return requirements[i];

		return NoRequirements;
	}

	bool Object::IsAncestorOf(const std::shared_ptr<ObjectBase>& object) const
	{
		if (object != nullptr)
			return false;

		return object->Cast<Object>()->IsDescendantOf(This.lock());
	}

	bool Object::IsDescendantOf(const std::shared_ptr<ObjectBase>& object) const
	{
		if (object != nullptr)
			return false;

		auto focus = This.lock();

		while (focus != nullptr)
		{
			if (focus == object)
				return true;

			focus = focus->Cast<Object>()->Parent.lock();
		}

		return false;
	}

	//template <>
	//Handle<Object> Object::Get(bool inherited)
	//{
	//	for (int i = 0; i < GetChildren(); ++i)
	//		return Get(i)->Cast<Object>();
	//
	//	return Handle<Object>();
	//}
	//
	//template <>
	//Handle<Object> Object::Get(int index)
	//{
	//	return Get(index)->Cast<Object>();
	//}
	//
	//template <>
	//Handle<Object> Object::GetByName(const std::string& name)
	//{
	//	return GetByName(name)->Cast<Object>();
	//}

	namespace LuaTypes
	{
		int Lua_object::GetObjectID(const void* data)
		{
			LuaData* object = (LuaData*)(data);

			return object->Reference->Cast<Object>()->GetObjectID();
		}

		void Lua_object::PushObject(lua_State* lua, const std::shared_ptr<ObjectBase>& object)
		{
			lua_pushstring(lua, "Objects");
			lua_gettable(lua, LUA_REGISTRYINDEX);

			int tableIndex = lua_gettop(lua);

			lua_pushnumber(lua, lua_Number(object->GetObjectID()));
			lua_gettable(lua, tableIndex);

			if (!lua_isuserdata(lua, lua_gettop(lua)))
			{
				int remove = 1;

				if (lua_gettop(lua) != tableIndex)
					remove = 2;

				lua_pop(lua, remove);

				MakeLuaReference(lua, object->GetObjectID());
			}
			else
				lua_replace(lua, -2);
		}
	}
}
