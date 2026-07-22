#pragma once
#include "CameraComponent.h"
#include "GameWorld.h"
#include "SpriteRenderComponent.h"
#include "RenderSystem.h"
#include "InputComponent.h"
#include "GameObject.h"

namespace Rogalique
{
	class Player
	{
	public:
		Player();
		GameEngine::GameObject* GetGameObject();
		void TryAttack(GameEngine::GameObject* target);

	private:
		GameEngine::GameObject* gameObject;
	};
}