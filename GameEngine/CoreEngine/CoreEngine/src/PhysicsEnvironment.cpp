#include "PhysicsEnvironment.h"

#include <queue>

#include "Transform.h"
#include "Collider2D.h"
#include "ColliderAsset.h"
#include "Terrain2DCollider.h"

namespace Engine
{
	namespace Physics
	{
		int RayCastResults::GetCount() const
		{
			return int(Colliders.size());
		}

		std::shared_ptr<Collider2D> RayCastResults::GetHit(int index) const
		{
			return Colliders[index];
		}

		float RayCastResults::GetDistance(int index) const
		{
			return Distances[index];
		}

		Vector3 RayCastResults::GetNormal(int index) const
		{
			return Normals[index];
		}

		void RayCastResults::Push(const std::shared_ptr<Collider2D>& collider, float distance, const Vector3& normal)
		{
			Colliders.push_back(collider);
			Distances.push_back(distance);
			Normals.push_back(normal);
		}

		PhysicsEnvironment::~PhysicsEnvironment()
		{
			for (int i = 0; i < int(Colliders.size()); ++i)
				ReferenceAllocator.Destroy(Colliders[i]);

			Colliders.clear();
		}

		void PhysicsEnvironment::Update(float delta)
		{
			for (int i = 0; i < int(Colliders.size()); ++i)
			{
				std::shared_ptr<Collider2D> collider = Colliders[i]->Collider.lock();

				collider->UpdateData();

				Aabb boundingBox = collider->GetBoundingBox();

				if (collider->HasMoved())
					ColliderPartitioning.Update(Colliders[i]->ID, boundingBox);
			}

			ColliderPartitioning.PairQuery([this] (const AabbTree::Node* node1, const AabbTree::Node* node2) {
				std::shared_ptr<Collider2D> collider1 = node1->GetData<ColliderReference>()->Collider.lock();
				std::shared_ptr<Collider2D> collider2 = node2->GetData<ColliderReference>()->Collider.lock();

				if (!collider1->CanCollideWith(collider2))
					return;

				std::shared_ptr<Transform> transform1 = collider1->GetComponent<Transform>();
				std::shared_ptr<Transform> transform2 = collider2->GetComponent<Transform>();

				if (transform1 == nullptr || transform2 == nullptr)
					return;

				CollisionPair collision;

				collision.Collider1 = collider1;
				collision.Collider2 = collider2;

				if (!collider1->IsCircle && !collider2->IsCircle)
				{
					Difference.Calculate(collider1->Data, collider2->Data);

					if (Difference.Colliding())
						collision.Displacement = -Difference.MinimumTranslationVector();
				}
				else if (!collider1->IsCircle && collider2->IsCircle)
				{
					CircleDifference.Calculate(collider1->Data, collider2->GetCenter(), collider2->GetRadius());

					int circle = -1;

					if (CircleDifference.Colliding(circle))
						collision.Displacement = -CircleDifference.MinimumTranslationVector(circle);
				}
				else if (collider1->IsCircle && !collider2->IsCircle)
				{
					CircleDifference.Calculate(collider2->Data, collider1->GetCenter(), collider1->GetRadius());

					int circle = -1;

					if (CircleDifference.Colliding(circle))
						collision.Displacement = CircleDifference.MinimumTranslationVector(circle);
				}
				else
				{
					float radiusSum = collider1->GetRadius() + collider2->GetRadius();

					if ((collider1->GetCenter() - collider2->GetCenter()).SquareLength() <= radiusSum * radiusSum)
						collision.Displacement = GetCircleMinimumTranslationVector(collider2->GetCenter() - collider1->GetCenter(), radiusSum);
				}

				CollisionBuffer.push_back(collision);
			});

			for (int i = 0; i < int(Terrains.size()); ++i)
			{
				auto terrain = Terrains[i].lock();

				terrain->PairQuery(std::as_const(ColliderPartitioning).GetRoot(),
					[this, &terrain] (const std::shared_ptr<Collider2D>& collider, const ColliderData& cell)
					{
						std::shared_ptr<Transform> transform = collider->GetComponent<Transform>();

						if (transform == nullptr)
							return;

						CollisionPair collision;

						collision.Collider1 = collider;
						//collision.Collider2 = terrain;

						if (collider->IsCircle)
						{
							CircleDifference.Calculate(cell, collider->GetCenter(), collider->GetRadius());

							int circle = -1;

							if (CircleDifference.Colliding(circle))
								collision.Displacement = CircleDifference.MinimumTranslationVector(circle);
						}
						else
						{
							Difference.Calculate(collider->Data, cell);

							if (Difference.Colliding())
								collision.Displacement = -Difference.MinimumTranslationVector();
						}

						CollisionBuffer.push_back(collision);
					},
					[] (void* data) -> std::shared_ptr<Physics::Collider2D>
					{
						return reinterpret_cast<ColliderReference*>(data)->Collider.lock();
					}
				);
			}

			for (int i = 0; i < int(CollisionBuffer.size()); ++i)
			{
				CollisionBuffer[i].Collider1->Collided(CollisionBuffer[i]);
				CollisionBuffer[i].Collider2->Collided(CollisionBuffer[i]);
			}

			CollisionBuffer.clear();
		}

		void PhysicsEnvironment::AddObject(const std::shared_ptr<Collider2D>& collider)
		{
			Aabb boundingBox;
			std::shared_ptr<Transform> transform = collider->GetComponent<Transform>();

			if (!collider->Asset.expired())
				boundingBox = collider->Asset.lock()->Data.BoundingBox;

			if (transform != nullptr)
				boundingBox = boundingBox.Transform(transform->GetWorldTransformation());

			ColliderReference* reference = ReferenceAllocator.Create<ColliderReference>();

			reference->Collider = collider;
			reference->ID = ColliderPartitioning.Insert(reference, boundingBox);

			Colliders.push_back(reference);
		}

		void PhysicsEnvironment::RemoveObject(const std::shared_ptr<Collider2D>& collider)
		{
			int index = -1;

			for (int i = 0; i < int(Colliders.size()); ++i)
				if (Colliders[i]->Collider.lock() == collider)
					index = i;

			if (index != -1)
			{
				ColliderPartitioning.Remove(Colliders[index]->ID);
				ReferenceAllocator.Destroy<>(Colliders[index]);

				std::swap(Colliders[index], Colliders.back());

				Colliders.pop_back();
			}
		}

		void PhysicsEnvironment::AddTerrain(const std::shared_ptr<Terrain2DCollider>& terrain)
		{
			Terrains.push_back(terrain);
		}

		void PhysicsEnvironment::RemoveTerrain(const std::shared_ptr<Terrain2DCollider>& terrain)
		{
			for (int i = 0; i < int(Terrains.size()); ++i)
			{
				std::shared_ptr<Terrain2DCollider> current = Terrains[i].lock();

				if (current == terrain)
				{
					std::swap(Terrains[i], Terrains.back());

					Terrains.pop_back();

					break;
				}
			}
		}

		namespace
		{
			struct CastResult
			{
				std::shared_ptr<Collider2D> Collider;
				float Distance = 0;
				Vector3 Normal;

				CastResult(std::shared_ptr<Collider2D> collider, float distance, Vector3 normal) : Collider(collider), Distance(distance), Normal(normal) {}

				bool operator<(const CastResult& other) const
				{
					return other.Distance < Distance;
				}
			};

			typedef std::priority_queue<CastResult> CastQueue;
		}

		RayCastResults PhysicsEnvironment::CastRay(const Ray& ray)
		{
			RayCastResults results;

			CastQueue hits;

			ColliderPartitioning.CastRay(ray, [&ray, &hits] (const AabbTree::Node* node, float t)
			{
				std::shared_ptr<Collider2D> collider = node->GetData<ColliderReference>()->Collider.lock();

				if (collider->IsCircle)
				{
					Vector3 center = collider->GetCenter();
					float radius = collider->GetRadius();

					if (VectorApproachesCollider(ray.Start, ray.Direction, center, radius * radius))
					{
						Vector3 offset = ray.Start - center;
						float a = ray.Direction * ray.Direction;
						float b = 2 * (ray.Direction * offset);
						float c = offset * offset - radius * radius;
						float distance = (-b - sqrtf(b * b - 4 * a * c)) / (2 * a);

						hits.push(CastResult(collider, distance, (center - ray.Start - distance * ray.Direction).Unit()));
					}
				}
				else if (collider->Asset.lock() != nullptr)
				{
					ColliderData& data = collider->Data;

					if (VectorApproachesCollider(ray.Start, ray.Direction, data.Center, data.RadiusSquared))
					{
						float distance = -1;
						int side = GetEntrySide(data, ray.Start, ray.Direction, distance);

						if (side != -1)
							hits.push(CastResult(collider, distance, data.Normals[side]));
					}
				}
			});

			while (!hits.empty())
			{
				const CastResult& hit = hits.top();

				results.Push(hit.Collider, hit.Distance, hit.Normal);

				hits.pop();
			}

			return results;
		}
	}
}