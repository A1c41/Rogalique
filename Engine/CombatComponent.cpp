#include "pch.h"
#include "CombatComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "Logger.h"

namespace GameEngine
{
    CombatComponent::CombatComponent(GameObject* gameObject) : Component(gameObject)
    {
        LOG_INFO("CombatComponent created for: " + gameObject->GetName());
    }

    void CombatComponent::Update(float fps)
    {
        if (currentCooldown > 0.0f)
        {
            currentCooldown -= fps;
            if (currentCooldown < 0.0f)
            {
                currentCooldown = 0.0f;
            }
        }
    }

    void CombatComponent::Render()
    {
    }

    void CombatComponent::SetDamage(float newDamage)
    {
        damage = newDamage;
    }

    float CombatComponent::GetDamage() const
    {
        return damage;
    }

    void CombatComponent::SetAttackRange(float range)
    {
        attackRange = range;
    }

    float CombatComponent::GetAttackRange() const
    {
        return attackRange;
    }

    void CombatComponent::SetAttackCooldown(float cooldown)
    {
        attackCooldown = cooldown;
    }

    float CombatComponent::GetAttackCooldown() const
    {
        return attackCooldown;
    }

    void CombatComponent::Attack(GameObject* target)
    {
        if (!CanAttack())
        {
            return;
        }

        if (!target)
        {
            LOG_WARN("Attack target is null");
            return;
        }

        if (!IsInRange(target))
        {
            return;
        }

        auto targetHealth = target->GetComponent<HealthComponent>();
        if (!targetHealth)
        {
            LOG_WARN("Target has no HealthComponent");
            return;
        }

        targetHealth->TakeDamage(damage);
        currentCooldown = attackCooldown;

        LOG_INFO(gameObject->GetName() + " attacked " + target->GetName() +
            " for " + std::to_string(damage) + " damage");

        if (onAttackCallback)
        {
            onAttackCallback(target);
        }
    }

    bool CombatComponent::CanAttack() const
    {
        return currentCooldown <= 0.0f;
    }

    bool CombatComponent::IsInRange(GameObject* target) const
    {
        if (!target)
        {
            return false;
        }

        auto myTransform = gameObject->GetComponent<TransformComponent>();
        auto targetTransform = target->GetComponent<TransformComponent>();

        if (!myTransform || !targetTransform)
        {
            return false;
        }

        Vector2Df myPos = myTransform->GetWorldPosition();
        Vector2Df targetPos = targetTransform->GetWorldPosition();
        Vector2Df diff = targetPos - myPos;
        float distance = diff.GetLength();

        return distance <= attackRange;
    }

    void CombatComponent::SetOnAttackCallback(std::function<void(GameObject*)> callback)
    {
        onAttackCallback = callback;
    }
}