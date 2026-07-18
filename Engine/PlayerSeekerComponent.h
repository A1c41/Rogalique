#pragma once

#include "framework.h"
#include "Component.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"

namespace GameEngine
{
    class ENGINE_API PlayerSeekerComponent : public Component
    {
    public:
        PlayerSeekerComponent(GameObject* gameObject);

        void Update(float fps) override;
        void Render() override;

        void SetTarget(GameObject* target);
        void SetDetectionRadius(float radius);
        void SetSpeed(float speed);

    private:
        TransformComponent* transform;
        RigidbodyComponent* rigidbody;
        GameObject* target;
        float detectionRadius = 200.0f;
        float speed = 100.0f;
        bool isChasing = false;

        Vector2Df GetDirectionToTarget() const;
        float GetDistanceToTarget() const;
    };
}