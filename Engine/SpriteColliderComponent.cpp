#include "pch.h"
#include "SpriteColliderComponent.h"
#include "GameObject.h"
#include "Logger.h"

namespace GameEngine
{
	SpriteColliderComponent::SpriteColliderComponent(GameObject* gameObject) : ColliderComponent(gameObject)
	{
		auto spriteRenderer = gameObject->GetComponent<SpriteRenderComponent>();
		if (spriteRenderer == nullptr)
		{
			LOG_ERROR("SpriteRenderer required for SpriteCollider on GameObject: " + gameObject->GetName());
			gameObject->RemoveComponent(this);
			throw std::runtime_error("SpriteColliderComponent requires SpriteRenderComponent");
		}

		sprite = spriteRenderer->GetSprite();
		ASSERT_MSG(sprite != nullptr, "Sprite is null after getting from SpriteRenderComponent");

		PhysicsSystem::Instance()->Subscribe(this);
		LOG_INFO("SpriteColliderComponent created for: " + gameObject->GetName());
	}

	SpriteColliderComponent::~SpriteColliderComponent()
	{
		if (&bounds != nullptr)
		{
			std::destroy_at(&bounds);
		}
		PhysicsSystem::Instance()->Unsubscribe(this);
	}

	void SpriteColliderComponent::Update(float deltaTime)
	{
		if (!sprite) {
			LOG_ERROR("Sprite is null in SpriteColliderComponent update");
			return;
		}
		bounds = sprite->getGlobalBounds();
	}

	void SpriteColliderComponent::Render()
	{
		if (!sprite) return;

		sf::RectangleShape rectangle(sf::Vector2f(bounds.width, bounds.height));
		rectangle.setPosition(bounds.left, bounds.top);
		rectangle.setFillColor(sf::Color::Transparent);
		rectangle.setOutlineColor(sf::Color::White);
		rectangle.setOutlineThickness(4);

		RenderSystem::Instance()->Render(rectangle);
	}
}