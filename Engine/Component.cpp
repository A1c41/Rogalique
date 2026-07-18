#include "pch.h"
#include "Component.h"
#include <iostream>

namespace GameEngine
{
	Component::Component(GameObject* gameObject) : gameObject(gameObject) {}

	Component::~Component()
	{
		std::cout << "Delete component: " << this << std::endl;
	}

	GameObject* Component::GetGameObject()
	{
		return gameObject;
	}
}