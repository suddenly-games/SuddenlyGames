#pragma once

#include "ObjectReflection.h"

#include "Object.h"

/*****
	TODO: Register new type to ObjectRegistration.cpp. Use the #include as a shortcut to open it.

	Remove this once registered.

	Warning: order of registration is crucial. Register this before any item that depends on it.
*****/

// #include "ObjectRegistration.cpp" // Remove this once registered

/*
	Quick note about reflected objects and Lua:
	Objects are expected to be passed around by reference using Handles and Lua will cache objects in its registry for lookup whenever they are referenced.
	If you need an item to be passed around by value consider turning it into a type instead of a game object.
	Only explicitly reflected items will be usable from Lua and file I/O. Reflection isn't a requirement for every item in a class.

	Object creation and internal Lua referencing is automatically handled, as well as object destruction.

	Objects will be automatically garbage collected if they have no references, so make sure that you don't leave dangling references to unneeded objects.

	Objects were built to be highly mutable to make them compatible with a scripting environment. If you need const-correctness to be respected, create
	a wrapper object to wrap around an internal class that respects the class's const correctness.

	Objects have a built in parent-child hierarchy, which makes them incredibly useful for easy book keeping.

	Objects do not need to have unique names per instance, even within the same parent.

	It is highly recommended to create a separate .meta.cpp file for each reflected enum, type, and object class.
*/

// Objects can be defined in any namespace as long as they are reflected within the Engine namespace
namespace ExampleNamespace
{
	class ObjectTemplate : public Engine::Object
	{
	public:
		// These 2 functions need to be defined. If they aren't defined then the parent class's versions will be called twice.
		void Initialize() {}
		void Update(float) {}

		// Allows creation with the object factory. Use "NonInstantiable" for singletons/base types.
		Instantiable;

		// This must match the parent type.
		Inherits_Class(Object);

		Reflected(ObjectTemplate);
	};
}

namespace Engine
{
	/*
		These must be in namespace Engine.
		These register class inheritance hierarchy meta data for Handles to use internally in type deduction.
		These are required to be used before using any Handles that reference that type.
		To make an object that contains handles that reference its own type, look at Object & ObjectBase's base class workaround.
	*/
	//Class_Inherits(ExampleNamespace::ObjectTemplate, Engine::Object);
}
