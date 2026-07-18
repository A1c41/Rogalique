#include "pch.h"
#include "SpriteRenderComponent.h"
#include "TransformComponent.h"
#include "RenderSystem.h"
#include "GameObject.h"

namespace GameEngine
{
	SpriteRenderComponent::SpriteRenderComponent(GameObject* gameObject) : Component(gameObject)
	{
		sprite = new sf::Sprite();
		sprite->setScale({ 1, 1 });
		transform = gameObject->GetComponent<TransformComponent>();
	}

	SpriteRenderComponent::~SpriteRenderComponent()
	{
		if (sprite != nullptr)
		{
			delete sprite;
		}
	}

	void SpriteRenderComponent::Update(float fps)
	{

	}

	void SpriteRenderComponent::Render()
	{
		if (sprite != nullptr)
		{
			auto pos = transform->GetWorldPosition();
			sprite->setPosition(pos.x, -pos.y);
			sprite->setRotation(-transform->GetWorldRotation());
			RenderSystem::Instance()->Render(*sprite);
		}
	}

	const sf::Sprite* SpriteRenderComponent::GetSprite() const
	{
		return sprite;
	}

	void SpriteRenderComponent::SetTexture(const sf::Texture& newTexture)
	{
		sprite->setTexture(newTexture);
		auto textureSize = sprite->getTexture()->getSize();
		sprite->setOrigin({ 0.5f * textureSize.x, 0.5f * textureSize.y });
	}

	void SpriteRenderComponent::SetPixelSize(int newWidth, int newHeight)
	{
		auto originalSize = sprite->getTexture()->getSize();
		sprite->setScale((float)newWidth / (float)originalSize.x, (float)newHeight / (float)originalSize.y);
	}

	void SpriteRenderComponent::FlipX(bool flip)
	{
		if (flip != isFlipX)
		{
			auto scale = sprite->getScale();
			sprite->setScale({ -scale.x, scale.y });
			isFlipX = flip;
		}
	}

	void SpriteRenderComponent::FlipY(bool flip)
	{
		if (flip != isFlipY)
		{
			auto scale = sprite->getScale();
			sprite->setScale({ scale.x, -scale.y });
			isFlipY = flip;
		}
	}
}