#include "pch.h"
#include "PhysicsSystem.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Collision.h"
#include "ColliderComponent.h"

namespace GameEngine
{
	PhysicsSystem* PhysicsSystem::Instance()
	{
		static PhysicsSystem physicsSystem;
		return &physicsSystem;
	}

	float PhysicsSystem::GetFixedDeltaTime() const
	{
		return fixedDeltaTime;
	}

    void PhysicsSystem::Update()
    {
        std::vector<std::pair<ColliderComponent*, ColliderComponent*>> processedCollisions;

        for (size_t i = 0; i < colliders.size(); i++)
        {
            for (size_t j = i + 1; j < colliders.size(); j++)
            {
                auto bodyA = colliders[i]->GetGameObject()->GetComponent<RigidbodyComponent>();
                auto bodyB = colliders[j]->GetGameObject()->GetComponent<RigidbodyComponent>();

                if (!bodyA || !bodyB)
                {
                    continue;
                }

                sf::FloatRect intersection;
                if (!colliders[i]->bounds.intersects(colliders[j]->bounds, intersection))
                {
                    continue;
                }

                bool alreadyProcessed = false;
                for (const auto& pair : processedCollisions)
                {
                    if ((pair.first == colliders[i] && pair.second == colliders[j]) ||
                        (pair.first == colliders[j] && pair.second == colliders[i]))
                    {
                        alreadyProcessed = true;
                        break;
                    }
                }
                if (alreadyProcessed) continue;

                processedCollisions.push_back(std::make_pair(colliders[i], colliders[j]));

                if (colliders[i]->isTrigger || colliders[j]->isTrigger)
                {
                    bool triggerExists = false;
                    for (const auto& pair : triggersEnteredPair)
                    {
                        if ((pair.first == colliders[i] && pair.second == colliders[j]) ||
                            (pair.first == colliders[j] && pair.second == colliders[i]))
                        {
                            triggerExists = true;
                            break;
                        }
                    }

                    if (!triggerExists)
                    {
                        Trigger trigger(colliders[i], colliders[j]);
                        colliders[i]->OnTriggerEnter(trigger);
                        colliders[j]->OnTriggerEnter(trigger);
                        triggersEnteredPair.push_back(std::make_pair(colliders[i], colliders[j]));
                    }
                }
                else
                {
                    TransformComponent* transformA = colliders[i]->GetGameObject()->GetComponent<TransformComponent>();
                    TransformComponent* transformB = colliders[j]->GetGameObject()->GetComponent<TransformComponent>();

                    bool aKinematic = bodyA->GetKinematic();
                    bool bKinematic = bodyB->GetKinematic();

                    if (aKinematic && bKinematic)
                    {
                        continue;
                    }

                    float halfWidth = intersection.width / 2.0f;
                    float halfHeight = intersection.height / 2.0f;

                    if (intersection.width < intersection.height)
                    {
                        if (!aKinematic)
                        {
                            if (colliders[i]->bounds.left < colliders[j]->bounds.left)
                            {
                                transformA->MoveBy({ -intersection.width, 0 });
                            }
                            else
                            {
                                transformA->MoveBy({ intersection.width, 0 });
                            }
                        }

                        if (!bKinematic)
                        {
                            if (colliders[j]->bounds.left < colliders[i]->bounds.left)
                            {
                                transformB->MoveBy({ -intersection.width, 0 });
                            }
                            else
                            {
                                transformB->MoveBy({ intersection.width, 0 });
                            }
                        }
                    }
                    else
                    {
                        if (!aKinematic)
                        {
                            if (colliders[i]->bounds.top < colliders[j]->bounds.top)
                            {
                                transformA->MoveBy({ 0, -intersection.height });
                            }
                            else
                            {
                                transformA->MoveBy({ 0, intersection.height });
                            }
                        }

                        if (!bKinematic)
                        {
                            if (colliders[j]->bounds.top < colliders[i]->bounds.top)
                            {
                                transformB->MoveBy({ 0, -intersection.height });
                            }
                            else
                            {
                                transformB->MoveBy({ 0, intersection.height });
                            }
                        }
                    }

                    Collision collision(colliders[i], colliders[j], intersection);
                    colliders[i]->OnCollision(collision);
                    colliders[j]->OnCollision(collision);
                }
            }
        }

        for (auto it = triggersEnteredPair.begin(); it != triggersEnteredPair.end(); )
        {
            if (!it->first->bounds.intersects(it->second->bounds))
            {
                Trigger trigger(it->first, it->second);
                it->first->OnTriggerExit(trigger);
                it->second->OnTriggerExit(trigger);
                it = triggersEnteredPair.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

	void PhysicsSystem::Subscribe(ColliderComponent* collider)
	{
		colliders.push_back(collider);
	}

	void PhysicsSystem::Unsubscribe(ColliderComponent* collider)
	{
		colliders.erase(std::remove_if(colliders.begin(), colliders.end(),
			[collider](ColliderComponent* obj) { return obj == collider; }),
			colliders.end());
	}
}