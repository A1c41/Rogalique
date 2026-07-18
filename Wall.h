#pragma once

#include "GameObject.h"
#include "SpriteRenderComponent.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "SpriteColliderComponent.h"
#include "GameWorld.h"

namespace Rogalique
{
    class Wall
    {
    public:
        Wall(const GameEngine::Vector2Df& position, const GameEngine::Vector2Df& size);
        ~Wall();

        GameEngine::GameObject* GetGameObject();

    private:
        GameEngine::GameObject* gameObject;
    };
}