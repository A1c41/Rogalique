#include "Player.h"
#include <ResourceSystem.h>

namespace Rogalique
{
	Player::Player()
	{
		gameObject = GameEngine::GameWorld::Instance()->CreateGameObject();
		auto playerRenderer = gameObject->AddComponent<GameEngine::SpriteRenderComponent>();

		playerRenderer->SetTexture(*GameEngine::ResourceSystem::Instance()->GetTextureShared("ball"));
		playerRenderer->SetPixelSize(32, 32);

		auto playerCamera = gameObject->AddComponent<GameEngine::CameraComponent>();
		playerCamera->SetWindow(&GameEngine::RenderSystem::Instance()->GetMainWindow());
		playerCamera->SetBaseResolution(1280, 720);

		auto playerInput = gameObject->AddComponent<GameEngine::InputComponent>();
	}

	GameEngine::GameObject* Player::GetGameObject()
	{
		return gameObject;
	}


}