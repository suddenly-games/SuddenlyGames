#include "Font.h"

#include "Texture.h"

namespace GraphicsEngine
{
	using Engine::Object;
	typedef Font::Character Character;

	Reflect_Type(Character,
		Document_Class("");
		
		Document("");
		Archivable Class_Member(char, Value);

		Document("");
		Archivable Class_Member(Vector3, TextOffset);

		Document("");
		Archivable Class_Member(Vector3, TextScale);

		Bind_Function(Character,
		
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
			
				Overload_Parameters();
			
				Bind_Constructor();
			);
		
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
			
				Overload_Parameters
				(
					Document("");
					Function_Parameter(char, character);

					Document("");
					Function_Parameter(float, aspectRatio);
					
					Document("");
					Function_Parameter(Vector3, textOffset);
					
					Document("");
					Function_Parameter(Vector3, textScale);

					Document("");
					Function_Parameter(float, kerningLeft);

					Document("");
					Function_Parameter(float, kerningRight);

					Document("");
					Function_Parameter(float, verticalOffset);
				);
			
				Bind_Constructor(character, aspectRatio, textOffset, textScale, kerningLeft, kerningRight, verticalOffset);
			);
		);
	);

	Reflect_Inherited(Font, Object,
		Document_Class("");
		
		Document("");
		Archivable Class_Member(float, SpaceWidth);

		Document("");
		Archivable Class_Member(int, TabSpaces);

		Bind_Function(Load,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::string, filePath);

					Document("");
					Function_Parameter(std::string, textureName);
				);

				Bind_Parameters_No_Return(Load, filePath, textureName);
			);
		);

		Bind_Function(GetCharacter,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Character);

				Overload_Parameters
				(
					Document("");
					Function_Parameter(char, character);
				);

				Bind_Parameters(GetCharacter, character);
			);
		);

		Bind_Function(GetTexture,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(std::shared_ptr<Texture>);

				Overload_Parameters();

				Bind_Parameters(GetTexture);
			);
		);

		Bind_Function(Load,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::string, path);

					Document("");
					Function_Parameter(std::string, textureName);
				);

				Bind_Parameters_No_Return(Load, path, textureName);
			);
		);
	);
}
