#include "Object.h"

namespace Engine
{
	Reflect_Inherited(Object, ObjectBase,
		Document_Class(
			"This type serves as the base type for all game objects. It uses ObjectBase as a "
			"reference point to form a parent-child hierarchy system. All game object types "
			"should inherit from this class or a sub class."
		);
		
		Document("Allows `GetComponent` to use siblings as components.");
		Archivable Class_Member(bool, SiblingComponents);

		Document("Allows `GetComponent` to use the object's parent as a component.");
		Archivable Class_Member(bool, ParentComponent);

		Document("Allows `GetComponent` to use ancestor objects as components.");
		Archivable Class_Member(bool, AncestorComponents);

		Document("Allows `GetComponent` to use the children of ancestor objects as components.");
		Archivable Class_Member(bool, SuperSiblingComponents);

		Document(
			"Specifies the maximum search distance up the ancestry chain past `Parent` "
			"for components in `GetComponent`. The value 0 effectively disables "
			"`AncestorComponents` and `SuperSiblingComponents`, while the value -1 "
			"allows the search to extend indefinitely until the root of the object "
			"hierarchy."
		);
		Archivable Class_Member(int, SuperComponentHeight);

		Bind_Function(GetFullName,

			Document("Gets the full object hierarchy path to the object.");
			Function_Overload
			(
				Document("The full path to the object from the hierarchy root.");
				Overload_Returns(std::string);

				Overload_Parameters();

				Bind_Parameters(GetFullName);
			);
		);

		Bind_Function(GetChildren,

			Document("Gets the number of children in the object");
			Function_Overload
			(
				Document("The number of the children that the object has.");
				Overload_Returns(int);

				Overload_Parameters();

				Bind_Parameters(GetChildren);
			);
		);
			
		Bind_Function(Get,

			Document("Gets the first child of with a given class type. Exact order isn't guaranteed.");
			Function_Overload
			(
				Document("The first child with the given type if it is found. Returns nil otherwise.");
				Overload_Returns(std::shared_ptr<ObjectBase>);

				Overload_Parameters
				(
					Document("The class name of the object to search for.");
					Function_Parameter(std::string, className);

					Document("Specifies whether derived types are allowed in the check or not.");
					Function_Parameter_Default(bool, inherits, true);
				);

				Bind_Parameters(Get, className, inherits);
			);

			Document("Gets the child at the given index.");
			Function_Overload
			(
				Document("The child at the given index. Returns nil if the index is out of bounds.");
				Overload_Returns(std::shared_ptr<ObjectBase>);

				Overload_Parameters
				(
					Document("The child index to look up.");
					Function_Parameter(int, index);
				);

				Bind_Parameters(Get, index);
			);
		);

		Bind_Function(GetByName,

			Document("Gets the first child with a given name. Exact order isn't guaranteed.");
			Function_Overload
			(
				Document("The first child with the given name if it is found. Returns nil otherwise.");
				Overload_Returns(std::shared_ptr<ObjectBase>);

				Overload_Parameters
				(
					Document("The name of the object to search for.");
					Function_Parameter(std::string, name);
				);

				Bind_Parameters(GetByName, name);
			);
		);

		Bind_Function(HasA,

			Document(
				"Checks if the object has a child with specified type, or optionally checks if it"
				" has a child with a derived type."
			);
			Function_Overload
			(
				Document("Returns true if this object or one of its children matches a given type.");
				Overload_Returns(bool);

				Overload_Parameters
				(
					Document("Checks for a child with this type or a derived type.");
					Function_Parameter(std::string, className);

					Document("Specifies whether derived types are allowed in the check or not.");
					Function_Parameter_Default(bool, inherits, true);
				);

				Bind_Parameters(HasA, className, inherits);
			);
		);

		Bind_Function(SetParent,

			Document("Sets the parent of the object to a new object.");
			Function_Overload
			(
				Returns_Nothing;
					
				Overload_Parameters
				(
					Document("Sets this as the object's new parent.");
					Function_Parameter(std::shared_ptr<ObjectBase>, newParent);
				);

				Bind_Parameters_No_Return(SetParent, newParent);
			);
		);

		Bind_Function(Remove,

			Document("Sets the parent of the object to null.");
			Function_Overload
			(
				Returns_Nothing;
					
				Overload_Parameters();

				Bind_Parameters_No_Return(Remove);
			);
		);

		Bind_Function(GetParent,

			Document("Retrieves the object's current parent.");
			Function_Overload
			(
				Document("A reference to the object's current parent.");
				Overload_Returns(std::shared_ptr<ObjectBase>);
					
				Overload_Parameters();

				Bind_Parameters(GetParent);
			);
		);

		Document("");
		Register_Lua_Property(Parent,
			Property_Getter(GetParent, std::shared_ptr<ObjectBase>);
			
			Property_Setters(
				Bind_Setter(SetParent, std::shared_ptr<ObjectBase>);
			);
		);

		Bind_Function(GetComponent,

			Document(
				"Retrieves a component from nearby in the game object hierarchy. The exact "
				"behavior can be specified using a set of members of the Object class with "
				"the following names:\n"
				"\tbool SiblingComponents = true;\n"
				"\tbool ParentComponent = true;\n"
				"\tbool AncestorComponents = true;\n"
				"\tbool SuperSiblingComponents = true;\n"
				"\tbool SuperComponentHeight = 0;\n\n"
				"Each of these flags enables extra behavior in this function and "
				"`SuperComponentHeight` specifies how far to search up the tree."
			);
			Function_Overload
			(
				Document("A reference to the first component that is found.");
				Overload_Returns(std::shared_ptr<ObjectBase>);
					
				Overload_Parameters
				(
					Document("Checks for a component with this type or a derived type.");
					Function_Parameter(std::string, className);

					Document("Specifies whether derived types are allowed in the check or not.");
					Function_Parameter_Default(bool, inherited, true);
				);

				Bind_Parameters(GetComponent, className, inherited);
			);
		);

		Bind_Function(CheckRestriction,

			Document(
				"Checks if this object restricts its parents to one object of its type, and if "
				"another object meets that restriction."
			);
			Function_Overload
			(
				Document("Returns true if this object can be moved into the other object based on copy restriction.");
				Overload_Returns(bool);

				Overload_Parameters
				(
					Document("The object that is being checked for requirements.");
					Function_Parameter(std::shared_ptr<ObjectBase>, newParent);
				);

				Bind_Parameters(CheckRestriction, newParent);
			);
		);

		Bind_Function(CheckRequirements,

			Document(
				"Checks if this object requires its parents to have specified sibling object "
				"types, and if another object meets those requirements."
			);
			Function_Overload
			(
				Document(
					"Returns the type name of a missing requirement if one is found. Returns an empty"
					" string otherwise."
				);
				Overload_Returns(std::string);

				Overload_Parameters
				(
					Document("The object that is being checked for requirements.");
					Function_Parameter(std::shared_ptr<ObjectBase>, newParent);
				);

				Bind_Parameters(CheckRequirements, newParent);
			);
		);

		Bind_Function(IsAncestorOf,

			Document("Checks if another object is inside of this object in the hierarchy.");
			Function_Overload
			(
				Document("Returns true if the object is a child or descendant of this object.");
				Overload_Returns(bool);

				Overload_Parameters
				(
					Document("The object who's ancestors is being checked.");
					Function_Parameter(std::shared_ptr<ObjectBase>, object);
				);

				Bind_Parameters(IsAncestorOf, object);
			);
		);

		Bind_Function(IsDescendantOf,

			Document("Checks if this object is inside of another object in the hierarchy.");
			Function_Overload
			(
				Document("Returns true if this object is a child or descendant of the given object.");
				Overload_Returns(bool);

				Overload_Parameters
				(
					Document("The object who is being compared against.");
					Function_Parameter(std::shared_ptr<ObjectBase>, object);
				);

				Bind_Parameters(IsDescendantOf, object);
			);
		);
	);
}
