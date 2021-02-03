#include "InputContext.h"

namespace GraphicsEngine
{
	Reflect_Inherited(InputContext, Object,
		Document_Class("");

		Document("");
		Archivable Class_Member(bool, Enabled);

		Document("");
		Archivable Class_Member(std::weak_ptr<InputSubscriber>, MouseFocus);

		Document("");
		Archivable Class_Member(std::weak_ptr<InputSubscriber>, InputFocus);

		Document("");
		Archivable Static Class_Member(std::weak_ptr<InputContext>, CurrentContext);
	);
}