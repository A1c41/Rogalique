#pragma once

#include "framework.h"
#include "TransformComponent.h"
#include <SFML/Graphics.hpp>
#include <memory>

namespace GameEngine
{
	class ENGINE_API SpriteRenderComponent : public Component
	{
	public:
		SpriteRenderComponent(GameObject* gameObject);
		~SpriteRenderComponent();

		void Update(float fps) override;
		void Render() override;

		const sf::Sprite* GetSprite() const;
		void SetTexture(const sf::Texture& newTexture);
		void SetPixelSize(int newWidth, int newHeight);

		void FlipX(bool flip);
		void FlipY(bool flip);

	private:
		sf::Sprite* sprite;
		TransformComponent* transform;

		bool isFlipX = false;
		bool isFlipY = false;
	};
}


