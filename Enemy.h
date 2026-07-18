#pragma once

#include "GameObject.h"
#include "SpriteRenderComponent.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "ColliderComponent.h"
#include "SpriteColliderComponent.h"
#include "PlayerSeekerComponent.h"
#include "GameWorld.h"

namespace Rogalique
{
    class Enemy
    {
    public:
        Enemy(GameEngine::GameObject* playerTarget, const GameEngine::Vector2Df& position);
        ~Enemy();

        GameEngine::GameObject* GetGameObject();
        void SetPosition(const GameEngine::Vector2Df& position);

    private:
        GameEngine::GameObject* gameObject;
    };
}