#pragma once

#include "GameObject.h"
#include "SpriteRenderComponent.h"
#include "TransformComponent.h"
#include "GameWorld.h"

namespace Rogalique
{
    class Floor
    {
    public:
        Floor(const GameEngine::Vector2Df& position, int tileSize);
        ~Floor();

        GameEngine::GameObject* GetGameObject();

    private:
        GameEngine::GameObject* gameObject;
    };
}