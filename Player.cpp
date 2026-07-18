#include "Player.h"
#include "ResourceSystem.h"
#include "SpriteColliderComponent.h"
#include "RigidbodyComponent.h"
#include "PhysicsSystem.h"

namespace Rogalique
{
    Player::Player()
    {
        gameObject = GameEngine::GameWorld::Instance()->CreateGameObject();

        auto playerRenderer = gameObject->AddComponent<GameEngine::SpriteRenderComponent>();
        auto texture = GameEngine::ResourceSystem::Instance()->GetTextureShared("hero");
        if (texture)
        {
            playerRenderer->SetTexture(*texture);
            playerRenderer->SetPixelSize(32, 32);
        }

        auto playerCamera = gameObject->AddComponent<GameEngine::CameraComponent>();
        playerCamera->SetWindow(&GameEngine::RenderSystem::Instance()->GetMainWindow());
        playerCamera->SetBaseResolution(1280, 720);

        auto playerInput = gameObject->AddComponent<GameEngine::InputComponent>();

        auto rigidbody = gameObject->AddComponent<GameEngine::RigidbodyComponent>();
        rigidbody->SetLinearDamping(10.0f);

        auto collider = gameObject->AddComponent<GameEngine::SpriteColliderComponent>();
        collider->SetTrigger(false);
    }

    GameEngine::GameObject* Player::GetGameObject()
    {
        return gameObject;
    }
}