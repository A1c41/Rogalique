#include "pch.h"
#include "InputComponent.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "SpriteRenderComponent.h"

namespace GameEngine
{
    InputComponent::InputComponent(GameObject* gameObject) : Component(gameObject)
    {

    }

    void InputComponent::Update(float fps)
    {
        verticalAxis = 0.f;
        horizontalAxis = 0.f;
        leftMousePressed = false;

        auto spriteRenderer = gameObject->GetComponent<SpriteRenderComponent>();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            verticalAxis += 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            verticalAxis -= 1.0f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            horizontalAxis += 1.0f;
            if (spriteRenderer)
            {
                spriteRenderer->FlipX(false);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            horizontalAxis -= 1.0f;
            if (spriteRenderer)
            {
                spriteRenderer->FlipX(true);
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            leftMousePressed = true;
        }

        if (horizontalAxis != 0.f && verticalAxis != 0.f)
        {
            float length = sqrtf(horizontalAxis * horizontalAxis + verticalAxis * verticalAxis);
            horizontalAxis /= length;
            verticalAxis /= length;
        }

        auto rigidbody = gameObject->GetComponent<RigidbodyComponent>();
        if (rigidbody)
        {
            float speed = 200.f;
            rigidbody->SetLinearVelocity({ horizontalAxis * speed * fps, verticalAxis * speed * fps });
        }
    }

    void InputComponent::Render()
    {

    }

    float InputComponent::GetHorizontalAxis() const
    {
        return horizontalAxis;
    }

    float InputComponent::GetVerticalAxis() const
    {
        return verticalAxis;
    }

    bool InputComponent::IsLeftMousePressed() const
    {
        return leftMousePressed;
    }
}