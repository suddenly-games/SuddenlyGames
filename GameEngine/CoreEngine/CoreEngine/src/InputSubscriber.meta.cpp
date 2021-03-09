#include "InputSubscriber.h"

namespace GraphicsEngine
{
	Reflect_Inherited(InputSubscription, InputObject,
		Document_Class("");

		Bind_Function(HasFocus,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);

				Overload_Parameters
				(
					Document("");
					Function_Parameter(LuaEnum<Enum::BoundDevice>, device);
				);

				Bind_Parameters(HasFocus, device);
			);
		);

		Bind_Function(Subscribe,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::shared_ptr<Engine::InputObject>, input);
				);

				Bind_Parameters_No_Return(Subscribe, input);
			);
		);
	);
	Reflect_Inherited(InputSubscriber, Object,
		Document_Class("");

		Bind_Function(HasFocus,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);

				Overload_Parameters
				(
					Document("");
					Function_Parameter(LuaEnum<Enum::BoundDevice>, device);
				);

				Bind_Parameters(HasFocus, device);
			);
		);

		Bind_Function(Subscribe,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(std::shared_ptr<InputSubscription>);
		
				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::shared_ptr<Engine::InputObject>, input);
				);
		
				Bind_Parameters(Subscribe, input);
			);
		);
	);
}