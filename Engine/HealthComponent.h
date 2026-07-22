#pragma once

#include "framework.h"
#include "Component.h"
#include "Vector.h"
#include <SFML/Graphics.hpp>

namespace GameEngine
{
    class GameObject;

    class ENGINE_API HealthComponent : public Component
    {
    public:
        HealthComponent(GameObject* gameObject);

        void Update(float fps) override;
        void Render() override;

        void SetMaxHealth(float maxHP);
        float GetMaxHealth() const;

        void SetCurrentHealth(float hp);
        float GetCurrentHealth() const;

        void SetArmor(float armorValue);
        float GetArmor() const;

        void TakeDamage(float damage);
        void Heal(float amount);

        bool IsAlive() const;
        float GetHealthPercentage() const;

        void SetShowHealthBar(bool show);
        void SetHealthBarOffset(const Vector2Df& offset);
        void SetHealthBarSize(const Vector2Df& size);
        void SetIsPlayer(bool isPlayer);

    private:
        float maxHealth = 100.0f;
        float currentHealth = 100.0f;
        float armor = 0.0f;

        bool showHealthBar = false;
        bool isPlayer = false;
        Vector2Df healthBarOffset = { 0.0f, -40.0f };
        Vector2Df healthBarSize = { 50.0f, 8.0f };

        sf::RectangleShape healthBarBackground;
        sf::RectangleShape healthBarFill;
        sf::RectangleShape armorBarBackground;
        sf::RectangleShape armorBarFill;
        sf::Text healthText;
        sf::Text armorText;
        sf::Font font;

        void UpdateHealthBarPosition();
        void RenderPlayerUI();
        void RenderEnemyHealthBar();
    };
}