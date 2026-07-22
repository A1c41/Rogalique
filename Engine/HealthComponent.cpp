#include "pch.h"
#include "HealthComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "RenderSystem.h"
#include "Logger.h"

namespace GameEngine
{
    HealthComponent::HealthComponent(GameObject* gameObject) : Component(gameObject)
    {
        LOG_INFO("HealthComponent created for: " + gameObject->GetName());

        healthBarBackground.setFillColor(sf::Color(60, 60, 60, 200));
        healthBarBackground.setOutlineColor(sf::Color::Black);
        healthBarBackground.setOutlineThickness(1.0f);

        healthBarFill.setFillColor(sf::Color::Red);

        armorBarBackground.setFillColor(sf::Color(60, 60, 60, 200));
        armorBarBackground.setOutlineColor(sf::Color::Black);
        armorBarBackground.setOutlineThickness(1.0f);

        armorBarFill.setFillColor(sf::Color(100, 100, 200, 200));
    }

    void HealthComponent::Update(float fps)
    {
        float healthPercent = GetHealthPercentage();

        if (healthPercent > 0.6f)
        {
            healthBarFill.setFillColor(sf::Color::Green);
        }
        else if (healthPercent > 0.3f)
        {
            healthBarFill.setFillColor(sf::Color::Yellow);
        }
        else
        {
            healthBarFill.setFillColor(sf::Color::Red);
        }

        UpdateHealthBarPosition();
    }

    void HealthComponent::Render()
    {
        if (!showHealthBar)
        {
            return;
        }

        if (isPlayer)
        {
            RenderPlayerUI();
        }
        else
        {
            RenderEnemyHealthBar();
        }
    }

    void HealthComponent::SetMaxHealth(float maxHP)
    {
        if (maxHP <= 0.0f)
        {
            LOG_WARN("Max health must be positive, setting to 1.0f");
            maxHP = 1.0f;
        }
        maxHealth = maxHP;
        if (currentHealth > maxHealth)
        {
            currentHealth = maxHealth;
        }
    }

    float HealthComponent::GetMaxHealth() const
    {
        return maxHealth;
    }

    void HealthComponent::SetCurrentHealth(float hp)
    {
        if (hp < 0.0f)
        {
            hp = 0.0f;
        }
        if (hp > maxHealth)
        {
            hp = maxHealth;
        }
        currentHealth = hp;
    }

    float HealthComponent::GetCurrentHealth() const
    {
        return currentHealth;
    }

    void HealthComponent::SetArmor(float armorValue)
    {
        if (armorValue < 0.0f)
        {
            armorValue = 0.0f;
        }
        armor = armorValue;
    }

    float HealthComponent::GetArmor() const
    {
        return armor;
    }

    void HealthComponent::TakeDamage(float damage)
    {
        if (damage <= 0.0f)
        {
            return;
        }

        float actualDamage = damage * (1.0f - armor / 100.0f);
        if (actualDamage < 0.0f)
        {
            actualDamage = 0.0f;
        }

        currentHealth -= actualDamage;
        if (currentHealth < 0.0f)
        {
            currentHealth = 0.0f;
        }

        LOG_INFO(gameObject->GetName() + " took " + std::to_string(actualDamage) +
            " damage. Health: " + std::to_string(currentHealth) + "/" + std::to_string(maxHealth));

        if (!IsAlive())
        {
            LOG_INFO(gameObject->GetName() + " died");
        }
    }

    void HealthComponent::Heal(float amount)
    {
        if (amount <= 0.0f)
        {
            return;
        }

        currentHealth += amount;
        if (currentHealth > maxHealth)
        {
            currentHealth = maxHealth;
        }

        LOG_INFO(gameObject->GetName() + " healed for " + std::to_string(amount) +
            ". Health: " + std::to_string(currentHealth) + "/" + std::to_string(maxHealth));
    }

    bool HealthComponent::IsAlive() const
    {
        return currentHealth > 0.0f;
    }

    float HealthComponent::GetHealthPercentage() const
    {
        if (maxHealth <= 0.0f)
        {
            return 0.0f;
        }
        return currentHealth / maxHealth;
    }

    void HealthComponent::SetShowHealthBar(bool show)
    {
        showHealthBar = show;
    }

    void HealthComponent::SetHealthBarOffset(const Vector2Df& offset)
    {
        healthBarOffset = offset;
    }

    void HealthComponent::SetHealthBarSize(const Vector2Df& size)
    {
        healthBarSize = size;
    }

    void HealthComponent::SetIsPlayer(bool player)
    {
        isPlayer = player;
    }

    void HealthComponent::UpdateHealthBarPosition()
    {
        auto transform = gameObject->GetComponent<TransformComponent>();
        if (!transform)
        {
            return;
        }

        Vector2Df worldPos = transform->GetWorldPosition();
        sf::Vector2f screenPos(worldPos.x + healthBarOffset.x, -worldPos.y + healthBarOffset.y);

        healthBarBackground.setPosition(screenPos);
        healthBarBackground.setSize(sf::Vector2f(healthBarSize.x, healthBarSize.y));

        float healthWidth = healthBarSize.x * GetHealthPercentage();
        healthBarFill.setPosition(screenPos);
        healthBarFill.setSize(sf::Vector2f(healthWidth, healthBarSize.y));

        armorBarBackground.setPosition(screenPos.x, screenPos.y + healthBarSize.y + 2.0f);
        armorBarBackground.setSize(sf::Vector2f(healthBarSize.x, healthBarSize.y * 0.7f));

        float armorWidth = healthBarSize.x * (armor / 100.0f);
        armorBarFill.setPosition(screenPos.x, screenPos.y + healthBarSize.y + 2.0f);
        armorBarFill.setSize(sf::Vector2f(armorWidth, healthBarSize.y * 0.7f));
    }

    void HealthComponent::RenderPlayerUI()
    {
        auto window = &RenderSystem::Instance()->GetMainWindow();
        sf::Vector2u windowSize = window->getSize();

        float barWidth = 250.0f;
        float barHeight = 20.0f;
        float padding = 20.0f;
        float x = padding;
        float y = windowSize.y - 60.0f;

        healthBarBackground.setPosition(x, y);
        healthBarBackground.setSize(sf::Vector2f(barWidth, barHeight));
        RenderSystem::Instance()->Render(healthBarBackground);

        float healthWidth = barWidth * GetHealthPercentage();
        healthBarFill.setPosition(x, y);
        healthBarFill.setSize(sf::Vector2f(healthWidth, barHeight));
        RenderSystem::Instance()->Render(healthBarFill);

        y = windowSize.y - 35.0f;
        armorBarBackground.setPosition(x, y);
        armorBarBackground.setSize(sf::Vector2f(barWidth, barHeight * 0.6f));
        RenderSystem::Instance()->Render(armorBarBackground);

        float armorWidth = barWidth * (armor / 100.0f);
        armorBarFill.setPosition(x, y);
        armorBarFill.setSize(sf::Vector2f(armorWidth, barHeight * 0.6f));
        RenderSystem::Instance()->Render(armorBarFill);
    }

    void HealthComponent::RenderEnemyHealthBar()
    {
        RenderSystem::Instance()->Render(healthBarBackground);
        RenderSystem::Instance()->Render(healthBarFill);
        RenderSystem::Instance()->Render(armorBarBackground);
        RenderSystem::Instance()->Render(armorBarFill);
    }
}