#pragma once

#include "ObjectReflection.h"

/*****
	TODO: Register new enum to ObjectRegistration.cpp. Use the #include as a shortcut to open it.

	Remove this once registered.

	Warning: order of registration is crucial. Register this before any item that depends on it.
*****/

// #include "ObjectRegistration.cpp" // Remove this once registered

// It is highly recommended to create a separate .meta.cpp file for each reflected enum, type, and object class.

namespace
{
	// Enum struct encapsulation to prevent namespace conflicts. Use typedefs to expose the inner enum.
	struct EnumTemplateEnum
	{
		enum EnumTemplate
		{
			Item1,
			Item2,
			Item3,
			Item4 = '6',
			Item5
		};
	};
}

/*
	Register the enum for ease of use in the rest of the engine.
	Required for reflected enums.
*/
namespace Enum
{
	typedef EnumTemplateEnum::EnumTemplate EnumTemplate;
}

namespace Engine
{
	Declare_Enum(EnumTemplate);
}
