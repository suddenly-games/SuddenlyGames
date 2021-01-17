#include "Object.h"

// "--------------------------------------------------------------------------------"
// 80 character string for formatting assistance

namespace Engine
{
	Reflect(ObjectBase,
		Document_Class(
			"The core data type behind all game objects. This type provides a basic interface"
			" and starting point for other game object types. It fails to provide a parent-"
			"child hierarchy because the object handle system requires an existing base type "
			"for definition."
		);

		Document("A non-unique name for an object. Used to document individual objects.");
		Archivable Class_Member(std::string, Name);

		Bind_Function(Update,
			Document(
				"Updates the game object based on the elapsed time. This object type's Update "
				"doesn't do anything on its own other than provide an interface for derived types"
				" to update."
			);
			Function_Overload
			(
				Returns_Nothing;

				Overload_Parameters
				(
					Document("The elapsed time that the object should use.");
					Function_Parameter(float, delta);
				);

				Bind_Parameters_No_Return(Update, delta);
			);
		);

		Bind_Function(GetTypeID,

			Document("Gets the ID of the type of the underlying object.");
			Function_Overload
			(
				Document("The object's type ID.");
				Overload_Returns(int);

				Overload_Parameters();

				Bind_Parameters(GetTypeID);
			);
		);

		Bind_Function(GetTypeName,

			Document("Gets the name of the underlying type of the object.");
			Function_Overload
			(
				Document("The object's type name.");
				Overload_Returns(std::string);

				Overload_Parameters();

				Bind_Parameters(GetTypeName);
			);
		);

		Bind_Function(IsA,

			Document(
				"Checks if the object is a specified type, or optionally checks if it is a "
				"derived type."
			);
			Function_Overload
			(
				Document("Returns if this object matches a given type.");
				Overload_Returns(bool);

				Overload_Parameters
				(
					Document("Checks if the object is this type or a derived type.");
					Function_Parameter(std::string, className);

					Document("Specifies whether derived types are allowed in the check or not.");
					Function_Parameter_Default(bool, inherits, true);
				);

				Bind_Parameters(IsA, className, inherits);
			);
		);

		Bind_Function(HasRequirements,

			Document(
				"Checks if the object has requirements on sibling object types. If it requires "
				"certain siblings and is placed in an object that doesn't contain them, then an "
				"error will be thrown."
			);
			Function_Overload
			(
				Document(
					"Returns true if another object has all of the object types required for this "
					"object to be added to it."
				);
				Overload_Returns(bool);

				Overload_Parameters();

				Bind_Parameters(HasRequirements);
			);
		);

		Bind_Function(operator string,

			Document("Returns the name of the object.");
			Function_Overload
			(
				Document("The name of the object.");
				Overload_Returns(std::string);

				Overload_Parameters();

				Bind_Parameters(operator std::string);
			);
		);
	);
}
