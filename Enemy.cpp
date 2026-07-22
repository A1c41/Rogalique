#include "Enemy.h"
#include "ResourceSystem.h"
#include "PhysicsSystem.h"
#include "HealthComponent.h"
#include "CombatComponent.h"
#include "Logger.h"

namespace Rogalique
{
    Enemy::Enemy(GameEngine::GameObject* playerTarget, const GameEngine::Vector2Df& position)
    {
        LOG_INFO("Creating enemy at position: (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")");

        ASSERT_MSG(playerTarget != nullptr, "Player target is null for enemy");

        gameObject = GameEngine::GameWorld::Instance()->CreateGameObject("Enemy");
        ASSERT_MSG(gameObject != nullptr, "Failed to create enemy GameObject");

        auto transform = gameObject->GetComponent<GameEngine::TransformComponent>();
        if (!transform) {
            LOG_ERROR("Enemy transform component not found");
            throw std::runtime_error("Enemy transform component not found");
        }
        transform->SetWorldPosition(position);

        auto renderer = gameObject->AddComponent<GameEngine::SpriteRenderComponent>();
        auto texture = GameEngine::ResourceSystem::Instance()->GetTextureShared("enemy");
        if (texture)
        {
            renderer->SetTexture(*texture);
            renderer->SetPixelSize(32, 32);
        }
        else
        {
            LOG_WARN("Enemy texture not found, enemy will be invisible");
        }

        auto rigidbody = gameObject->AddComponent<GameEngine::RigidbodyComponent>();
        rigidbody->SetLinearDamping(5.0f);

        auto collider = gameObject->AddComponent<GameEngine::SpriteColliderComponent>();
        collider->SetTrigger(false);

        auto seeker = gameObject->AddComponent<GameEngine::PlayerSeekerComponent>();
        seeker->SetTarget(playerTarget);
        seeker->SetDetectionRadius(300.0f);
        seeker->SetSpeed(150.0f);

        auto health = gameObject->AddComponent<GameEngine::HealthComponent>();
        if (!health) {
            LOG_ERROR("Failed to add HealthComponent to enemy");
            throw std::runtime_error("Failed to add HealthComponent to enemy");
        }
        health->SetMaxHealth(50.0f);
        health->SetCurrentHealth(50.0f);
        health->SetArmor(5.0f);
        health->SetShowHealthBar(true);
        health->SetHealthBarOffset({ -25.0f, -30.0f });
        health->SetHealthBarSize({ 50.0f, 6.0f });
        health->SetIsPlayer(false);

        auto combat = gameObject->AddComponent<GameEngine::CombatComponent>();
        if (!combat) {
            LOG_ERROR("Failed to add CombatComponent to enemy");
            throw std::runtime_error("Failed to add CombatComponent to enemy");
        }
        combat->SetDamage(10.0f);
        combat->SetAttackRange(40.0f);
        combat->SetAttackCooldown(2.0f);

        LOG_INFO("Enemy created successfully with HP: " + std::to_string(health->GetCurrentHealth()) +
            " Armor: " + std::to_string(health->GetArmor()));
    }

    Enemy::~Enemy()
    {
    }

    GameEngine::GameObject* Enemy::GetGameObject()
    {
        return gameObject;
    }

    void Enemy::SetPosition(const GameEngine::Vector2Df& position)
    {
        if (gameObject)
        {
            auto transform = gameObject->GetComponent<GameEngine::TransformComponent>();
            if (transform)
            {
                transform->SetWorldPosition(position);
            }
            else
            {
                LOG_WARN("Enemy transform component not found for position update");
            }
        }
        else
        {
            LOG_ERROR("Cannot set position on destroyed enemy");
        }
    }
}