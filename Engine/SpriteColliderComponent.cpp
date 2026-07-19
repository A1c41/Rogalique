#include "pch.h"
#include "SpriteColliderComponent.h"
#include "GameObject.h"

namespace GameEngine
{
	SpriteColliderComponent::SpriteColliderComponent(GameObject* gameObject) : ColliderComponent(gameObject)
	{
		auto spriteRenderer = gameObject->GetComponent<SpriteRenderComponent>();
		if (spriteRenderer == nullptr)
		{
			std::cout << "SpriteRenderer required to SpriteCollider." << std::endl;
			gameObject->RemoveComponent(this);
			return;
		}

		sprite = gameObject->GetComponent<SpriteRenderComponent>()->GetSprite();
		PhysicsSystem::Instance()->Subscribe(this);
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
		bounds = sprite->getGlobalBounds();
	}
	
	void SpriteColliderComponent::Render()
	{
		sf::RectangleShape rectangle(sf::Vector2f(bounds.width, bounds.height));
		rectangle.setPosition(bounds.left, bounds.top);
		rectangle.setFillColor(sf::Color::Transparent);
		rectangle.setOutlineColor(sf::Color::White);
		rectangle.setOutlineThickness(4);

		RenderSystem::Instance()->Render(rectangle);
	}
}