#include "CollisionGroup.h"

namespace Engine
{
	namespace Physics
	{
		CollisionGroup::~CollisionGroup()
		{
			for (GroupMap::iterator i = Interactions.begin(); i != Interactions.end(); ++i)
			{
				std::shared_ptr<CollisionGroup> other(i->second.first.lock()->Cast<CollisionGroup>());

				if (other != nullptr)
					other->Interactions.erase(This.lock()->GetObjectID());
			}
		}

		void CollisionGroup::AddInteraction(const std::shared_ptr<Object>& group, InteractionType type)
		{
			AddInteractionRegistry(group, type);
			group->Cast<CollisionGroup>()->AddInteractionRegistry(This.lock()->Cast<CollisionGroup>(), type);
		}

		Enum::InteractionType CollisionGroup::GetInteraction(const std::shared_ptr<Object>& group) const
		{
			GroupMap::const_iterator i = Interactions.find(group->GetObjectID());

			if (i != Interactions.end())
				return i->second.second;

			return InteractionType::None;
		}

		void CollisionGroup::AddInteractionRegistry(const std::shared_ptr<Object>& group, InteractionType type)
		{
			Interactions[group->GetObjectID()] = std::make_pair(std::weak_ptr<Object>(group), type);
		}
	}
}