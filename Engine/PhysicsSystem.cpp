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

                    sf::Vector2f centerA(colliders[i]->bounds.left + colliders[i]->bounds.width / 2.0f,
                        colliders[i]->bounds.top + colliders[i]->bounds.height / 2.0f);
                    sf::Vector2f centerB(colliders[j]->bounds.left + colliders[j]->bounds.width / 2.0f,
                        colliders[j]->bounds.top + colliders[j]->bounds.height / 2.0f);

                    sf::Vector2f direction = centerB - centerA;

                    if (intersection.width < intersection.height)
                    {
                        float sign = (direction.x > 0) ? 1.0f : -1.0f;

                        if (!aKinematic)
                        {
                            transformA->MoveBy({ -sign * intersection.width, 0 });
                            Vector2Df vel = bodyA->GetLinearVelocity();
                            vel.x = 0;
                            bodyA->SetLinearVelocity(vel);
                        }
                        if (!bKinematic)
                        {
                            transformB->MoveBy({ sign * intersection.width, 0 });
                            Vector2Df vel = bodyB->GetLinearVelocity();
                            vel.x = 0;
                            bodyB->SetLinearVelocity(vel);
                        }
                    }
                    else
                    {
                        float sign = (direction.y > 0) ? 1.0f : -1.0f;

                        if (!aKinematic)
                        {
                            transformA->MoveBy({ 0, sign * intersection.height });
                            Vector2Df vel = bodyA->GetLinearVelocity();
                            vel.y = 0;
                            bodyA->SetLinearVelocity(vel);
                        }
                        if (!bKinematic)
                        {
                            transformB->MoveBy({ 0, -sign * intersection.height });
                            Vector2Df vel = bodyB->GetLinearVelocity();
                            vel.y = 0;
                            bodyB->SetLinearVelocity(vel);
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