#include "Player.h"
#include "ResourceSystem.h"
#include "SpriteColliderComponent.h"
#include "RigidbodyComponent.h"
#include "PhysicsSystem.h"
#include "Logger.h"

namespace Rogalique
{
    Player::Player()
    {
        LOG_INFO("Creating player");

        gameObject = GameEngine::GameWorld::Instance()->CreateGameObject("Player");
        ASSERT_MSG(gameObject != nullptr, "Failed to create player GameObject");

        auto playerRenderer = gameObject->AddComponent<GameEngine::SpriteRenderComponent>();
        if (!playerRenderer) {
            LOG_ERROR("Failed to add SpriteRenderComponent to player");
            throw std::runtime_error("Failed to add SpriteRenderComponent to player");
        }

        auto texture = GameEngine::ResourceSystem::Instance()->GetTextureShared("hero");
        if (texture)
        {
            playerRenderer->SetTexture(*texture);
            playerRenderer->SetPixelSize(32, 32);
        }
        else
        {
            LOG_ERROR("Hero texture not loaded");
            throw std::runtime_error("Hero texture not loaded");
        }

        auto playerCamera = gameObject->AddComponent<GameEngine::CameraComponent>();
        if (!playerCamera) {
            LOG_ERROR("Failed to add CameraComponent to player");
            throw std::runtime_error("Failed to add CameraComponent to player");
        }
        playerCamera->SetWindow(&GameEngine::RenderSystem::Instance()->GetMainWindow());
        playerCamera->SetBaseResolution(1280, 720);

        auto playerInput = gameObject->AddComponent<GameEngine::InputComponent>();
        if (!playerInput) {
            LOG_ERROR("Failed to add InputComponent to player");
            throw std::runtime_error("Failed to add InputComponent to player");
        }

        auto rigidbody = gameObject->AddComponent<GameEngine::RigidbodyComponent>();
        if (!rigidbody) {
            LOG_ERROR("Failed to add RigidbodyComponent to player");
            throw std::runtime_error("Failed to add RigidbodyComponent to player");
        }
        rigidbody->SetLinearDamping(10.0f);

        auto collider = gameObject->AddComponent<GameEngine::SpriteColliderComponent>();
        if (!collider) {
            LOG_ERROR("Failed to add SpriteColliderComponent to player");
            throw std::runtime_error("Failed to add SpriteColliderComponent to player");
        }
        collider->SetTrigger(false);

        LOG_INFO("Player created successfully");
    }

    GameEngine::GameObject* Player::GetGameObject()
    {
        ASSERT_MSG(gameObject != nullptr, "Player GameObject is null");
        return gameObject;
    }
}