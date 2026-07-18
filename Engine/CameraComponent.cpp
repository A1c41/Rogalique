#include "pch.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

namespace GameEngine
{
	CameraComponent::CameraComponent(GameObject* gameObject) : Component(gameObject)
	{
		view = new sf::View(sf::FloatRect(0, 0, 800, 600));
		transform = gameObject->GetComponent<TransformComponent>();
	}

	CameraComponent::~CameraComponent()
	{
		delete view;
	}

	void CameraComponent::Update(float fps)
	{
		auto position = transform->GetWorldPosition();

		sf::Vector2f sfmlPos;
		sfmlPos.x = position.x;
		sfmlPos.y = -position.y;

		view->setCenter(sfmlPos);
		view->setRotation(-transform->GetWorldRotation());

		window->setView(*view);
	}

	void CameraComponent::Render()
	{
		if (window == nullptr)
		{
			std::cout << "NULL window render." << std::endl;
		}
	}

	void CameraComponent::SetWindow(sf::RenderWindow* newWindow)
	{
		window = newWindow;
	}

	void CameraComponent::ZoomBy(float newZoom)
	{
		if (newZoom <= 0)
		{
			std::cout << "Not allowed zoom lesser or equal than zero." << std::endl;
			return;
		}
		view->zoom(newZoom);
	}

	void CameraComponent::SetBaseResolution(int width, int height)
	{
		view->reset(sf::FloatRect(0, 0, width, height));
	}
}