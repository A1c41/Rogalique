#pragma once

#include "framework.h"
#include <SFML/Graphics.hpp>

namespace GameEngine
{
	class ColliderComponent;

	class ENGINE_API Collision
	{
	public:
		Collision(ColliderComponent* newFirst, ColliderComponent* newSecond, sf::FloatRect newCollisionRect) :
			first(newFirst), second(newSecond), collisionRect(newCollisionRect) {
		};
	private:
		ColliderComponent* first;
		ColliderComponent* second;
		sf::FloatRect collisionRect;
	};
}
