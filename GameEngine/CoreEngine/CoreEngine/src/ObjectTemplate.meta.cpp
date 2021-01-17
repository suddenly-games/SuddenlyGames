#include "ObjectTemplate.h"

// use these as shortcuts to open the other templates with reflection examples
// #include "TypeTemplate.meta.cpp"
// #include "InheritedTemplate.meta.cpp"

namespace ExampleNamespace
{
	using Engine::Object;

	Reflect_Inherited(ObjectTemplate, Object,
		Document_Class(
			"-------------------------------------------------------------------------------"
			"This is a template for future game object types. It is also an example of a "
			"good practice for documenting items. I like to use this 80 character bar of "
			"hyphens as a reference point for how long to make a single line and use c "
			"style literal string concatenation to split apart lines. Please do this if "
			"the documentation goes over 100 or so characters to keep things nice and "
			"tidy."
		);

		// To see an example of how to configure reflection data check "TypeTemplate.meta.cpp and InheritedTemplate.meta.cpp"
	);
}
