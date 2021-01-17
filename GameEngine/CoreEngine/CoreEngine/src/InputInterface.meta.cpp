#include "InputInterface.h"

namespace GraphicsEngine
{
	Reflect_Inherited(InputInterface, Object,
		Document_Class("");

		Document("");
		Archivable Class_Member(bool, Enabled);

		Document("");
		Archivable Class_Member(std::weak_ptr<Engine::Object>, UpConnection);

		Document("");
		Archivable Class_Member(std::weak_ptr<Engine::Object>, DownConnection);

		Document("");
		Archivable Class_Member(std::weak_ptr<Engine::Object>, LeftConnection);

		Document("");
		Archivable Class_Member(std::weak_ptr<Engine::Object>, RightConnection);
	);
}