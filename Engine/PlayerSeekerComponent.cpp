#include "pch.h"
#include "PlayerSeekerComponent.h"
#include <cmath>

namespace GameEngine
{
    PlayerSeekerComponent::PlayerSeekerComponent(GameObject* gameObject)
        : Component(gameObject)
    {
        transform = gameObject->GetComponent<TransformComponent>();
        rigidbody = gameObject->GetComponent<RigidbodyComponent>();
    }

    void PlayerSeekerComponent::Update(float fps)
    {
        if (!target || !transform || !rigidbody)
            return;

        float distance = GetDistanceToTarget();

        if (distance < detectionRadius)
        {
            isChasing = true;
            Vector2Df direction = GetDirectionToTarget();
            rigidbody->SetLinearVelocity(direction * speed);
        }
        else
        {
            isChasing = false;
            rigidbody->SetLinearVelocity({ 0, 0 });
        }
    }

    void PlayerSeekerComponent::Render()
    {
        
    }

    void PlayerSeekerComponent::SetTarget(GameObject* newTarget)
    {
        target = newTarget;
    }

    void PlayerSeekerComponent::SetDetectionRadius(float radius)
    {
        detectionRadius = radius;
    }

    void PlayerSeekerComponent::SetSpeed(float newSpeed)
    {
        speed = newSpeed;
    }

    Vector2Df PlayerSeekerComponent::GetDirectionToTarget() const
    {
        if (!target || !transform)
            return { 0, 0 };

        Vector2Df targetPos = target->GetComponent<TransformComponent>()->GetWorldPosition();
        Vector2Df myPos = transform->GetWorldPosition();
        Vector2Df direction = targetPos - myPos;

        float length = direction.GetLength();
        if (length > 0)
        {
            return { direction.x / length, direction.y / length };
        }
        return { 0, 0 };
    }

    float PlayerSeekerComponent::GetDistanceToTarget() const
    {
        if (!target || !transform)
            return 0;

        Vector2Df targetPos = target->GetComponent<TransformComponent>()->GetWorldPosition();
        Vector2Df myPos = transform->GetWorldPosition();
        return (targetPos - myPos).GetLength();
    }
}