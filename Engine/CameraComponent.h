#pragma once

#include "framework.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "TransformComponent.h"

namespace GameEngine
{
	class ENGINE_API CameraComponent : public Component
	{
	public:
		CameraComponent(GameObject* gameObject);
		~CameraComponent();

		void Update(float fps) override;
		void Render() override;

		void SetWindow(sf::RenderWindow* newWindow);
		void ZoomBy(float newZoom);

		void SetBaseResolution(int width, int height);

	private:
		TransformComponent* transform;
		sf::RenderWindow* window;
		sf::View* view;
	};
}