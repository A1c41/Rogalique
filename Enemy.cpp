#include "Enemy.h"
#include "ResourceSystem.h"
#include "PhysicsSystem.h"

namespace Rogalique
{
    Enemy::Enemy(GameEngine::GameObject* playerTarget, const GameEngine::Vector2Df& position)
    {
        gameObject = GameEngine::GameWorld::Instance()->CreateGameObject();

        auto transform = gameObject->GetComponent<GameEngine::TransformComponent>();
        transform->SetWorldPosition(position);

        auto renderer = gameObject->AddComponent<GameEngine::SpriteRenderComponent>();
        auto texture = GameEngine::ResourceSystem::Instance()->GetTextureShared("enemy");
        if (texture)
        {
            renderer->SetTexture(*texture);
            renderer->SetPixelSize(32, 32);
        }

        auto rigidbody = gameObject->AddComponent<GameEngine::RigidbodyComponent>();
        rigidbody->SetLinearDamping(5.0f);

        auto collider = gameObject->AddComponent<GameEngine::SpriteColliderComponent>();
        collider->SetTrigger(false);

        auto seeker = gameObject->AddComponent<GameEngine::PlayerSeekerComponent>();
        seeker->SetTarget(playerTarget);
        seeker->SetDetectionRadius(300.0f);
        seeker->SetSpeed(150.0f);
    }

    Enemy::~Enemy()
    {
        if (gameObject)
        {
            GameEngine::GameWorld::Instance()->DestroyGameObject(gameObject);
        }
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
        }
    }
}