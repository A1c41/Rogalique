#include "Floor.h"
#include "ResourceSystem.h"

namespace Rogalique
{
    Floor::Floor(const GameEngine::Vector2Df& position, int tileSize)
    {
        gameObject = GameEngine::GameWorld::Instance()->CreateGameObject();

        auto transform = gameObject->GetComponent<GameEngine::TransformComponent>();
        transform->SetWorldPosition(position);

        auto renderer = gameObject->AddComponent<GameEngine::SpriteRenderComponent>();
        auto texture = GameEngine::ResourceSystem::Instance()->GetTextureShared("floor");
        if (texture)
        {
            renderer->SetTexture(*texture);
            renderer->SetPixelSize(tileSize, tileSize);
        }
    }

    Floor::~Floor()
    {
        if (gameObject)
        {
            GameEngine::GameWorld::Instance()->DestroyGameObject(gameObject);
        }
    }

    GameEngine::GameObject* Floor::GetGameObject()
    {
        return gameObject;
    }
}