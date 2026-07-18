#include "Wall.h"
#include "ResourceSystem.h"
#include "PhysicsSystem.h"

namespace Rogalique
{
    Wall::Wall(const GameEngine::Vector2Df& position, const GameEngine::Vector2Df& size)
    {
        gameObject = GameEngine::GameWorld::Instance()->CreateGameObject();

        auto transform = gameObject->GetComponent<GameEngine::TransformComponent>();
        transform->SetWorldPosition(position);
        transform->SetWorldScale(size);

        auto renderer = gameObject->AddComponent<GameEngine::SpriteRenderComponent>();
        auto texture = GameEngine::ResourceSystem::Instance()->GetTextureShared("wall");
        if (texture)
        {
            renderer->SetTexture(*texture);
            renderer->SetPixelSize(static_cast<int>(size.x), static_cast<int>(size.y));
        }

        auto rigidbody = gameObject->AddComponent<GameEngine::RigidbodyComponent>();
        rigidbody->SetKinematic(true);

        auto collider = gameObject->AddComponent<GameEngine::SpriteColliderComponent>();
        collider->SetTrigger(false);
    }

    Wall::~Wall()
    {
        if (gameObject)
        {
            GameEngine::GameWorld::Instance()->DestroyGameObject(gameObject);
        }
    }

    GameEngine::GameObject* Wall::GetGameObject()
    {
        return gameObject;
    }
}