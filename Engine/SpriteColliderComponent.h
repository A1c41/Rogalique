#pragma once

#include "framework.h"
#include "ColliderComponent.h"
#include "SpriteRenderComponent.h"
#include "RenderSystem.h"
#include <SFML/Graphics/Sprite.hpp>

namespace GameEngine
{
	class ENGINE_API SpriteColliderComponent : public ColliderComponent
	{
	public:
		SpriteColliderComponent(GameObject* gameObject);
		~SpriteColliderComponent();

		void Update(float deltaTime) override;
		void Render() override;
	private:
		const sf::Sprite* sprite;
	};
}