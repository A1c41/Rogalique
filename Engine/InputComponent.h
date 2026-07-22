#pragma once

#include "framework.h"
#include "Component.h"
#include <SFML/Window.hpp>

namespace GameEngine
{
    class ENGINE_API InputComponent : public Component
    {
    public:
        InputComponent(GameObject* gameObject);

        void Update(float fps) override;
        void Render() override;

        float GetHorizontalAxis() const;
        float GetVerticalAxis() const;
        bool IsLeftMousePressed() const;

    private:
        float horizontalAxis = 0.f;
        float verticalAxis = 0.f;
        bool leftMousePressed = false;
    };
}