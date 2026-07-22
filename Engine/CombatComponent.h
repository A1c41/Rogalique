#pragma once

#include "framework.h"
#include "Component.h"
#include "Vector.h"
#include <functional>

namespace GameEngine
{
    class GameObject;

    class ENGINE_API CombatComponent : public Component
    {
    public:
        CombatComponent(GameObject* gameObject);

        void Update(float fps) override;
        void Render() override;

        void SetDamage(float damage);
        float GetDamage() const;

        void SetAttackRange(float range);
        float GetAttackRange() const;

        void SetAttackCooldown(float cooldown);
        float GetAttackCooldown() const;

        void Attack(GameObject* target);
        bool CanAttack() const;
        bool IsInRange(GameObject* target) const;

        void SetOnAttackCallback(std::function<void(GameObject*)> callback);

    private:
        float damage = 10.0f;
        float attackRange = 50.0f;
        float attackCooldown = 1.0f;
        float currentCooldown = 0.0f;

        std::function<void(GameObject*)> onAttackCallback;
    };
}