#include "DeveloperLevel.h"
#include "GameWorld.h"
#include "GameObject.h"
#include "SoundSystem.h"
#include "ResourceSystem.h"
#include "CombatComponent.h"
#include "HealthComponent.h"
#include "InputComponent.h"
#include "TransformComponent.h"
#include "Logger.h"

using namespace GameEngine;

namespace Rogalique
{
    DeveloperLevel::DeveloperLevel()
    {
        LOG_INFO("DeveloperLevel constructor");
        levelGenerator = std::make_shared<LevelGenerator>();
        levelGenerator->LoadLevels("Resources/Levels");
    }

    void DeveloperLevel::Start()
    {
        LOG_INFO("Starting developer level");

        SoundSystem::Instance()->LoadMusic("background", "Resources//background_music.ogg");
        SoundSystem::Instance()->PlayMusic("background", true);
        SoundSystem::Instance()->SetMusicVolume("background", 70.0f);

        LevelData levelData = levelGenerator->GetRandomLevel();

        if (!levelGenerator->GenerateLevelFromFile(levelData))
        {
            LOG_WARN("Failed to generate level from file, generating default level");
            levelGenerator->GenerateLevel(playerStartPosition, 25, 15, 32);
        }

        Vector2Df playerPos = levelGenerator->GetPlayerStart();
        if (playerPos.x == 0 && playerPos.y == 0)
        {
            LOG_WARN("Player start position not set, using default");
            playerPos = playerStartPosition;
        }

        player = std::make_shared<Player>();

        auto playerTransform = player->GetGameObject()->GetComponent<TransformComponent>();
        if (playerTransform)
        {
            playerTransform->SetWorldPosition(playerPos);
            LOG_INFO("Player placed at: (" + std::to_string(playerPos.x) + ", " + std::to_string(playerPos.y) + ")");
        }

        auto playerHealth = player->GetGameObject()->GetComponent<HealthComponent>();
        if (playerHealth)
        {
            playerHealth->SetCurrentHealth(playerHealth->GetMaxHealth());
        }

        SpawnEnemies();

        LOG_INFO("Developer level started with " + std::to_string(enemies.size()) + " enemies");
    }

    void DeveloperLevel::Restart()
    {
        LOG_INFO("Restarting developer level");

        enemies.clear();

        levelGenerator->Clear();

        player.reset();

        GameWorld::Instance()->Clear();

        Start();
    }

    void DeveloperLevel::Stop()
    {
        LOG_INFO("Stopping developer level");
        SoundSystem::Instance()->StopMusic("background");

        enemies.clear();

        player.reset();

        levelGenerator->Clear();

        GameWorld::Instance()->Clear();

        LOG_INFO("Developer level stopped");
    }

    void DeveloperLevel::Update(float fps)
    {
        if (!player)
        {
            return;
        }

        auto playerGameObject = player->GetGameObject();
        if (!playerGameObject)
        {
            return;
        }

        auto playerHealth = playerGameObject->GetComponent<HealthComponent>();
        if (!playerHealth)
        {
            return;
        }

        if (!playerHealth->IsAlive())
        {
            LOG_INFO("Player died, restarting level");
            Restart();
            return;
        }

        HandlePlayerAttack();
        HandleEnemyAttacks();

        for (auto it = enemies.begin(); it != enemies.end();)
        {
            if (!(*it))
            {
                it = enemies.erase(it);
                continue;
            }

            auto enemyGameObject = (*it)->GetGameObject();
            if (!enemyGameObject)
            {
                it = enemies.erase(it);
                continue;
            }

            auto enemyHealth = enemyGameObject->GetComponent<HealthComponent>();
            if (enemyHealth && !enemyHealth->IsAlive())
            {
                LOG_INFO("Removing dead enemy");
                GameWorld::Instance()->DestroyGameObject(enemyGameObject);
                it = enemies.erase(it);
            }
            else
            {
                ++it;
            }
        }

        if (enemies.empty())
        {
            LOG_INFO("All enemies defeated, generating new level");

            enemies.clear();

            levelGenerator->Clear();

            player.reset();

            GameWorld::Instance()->Clear();

            Start();
        }
    }

    void DeveloperLevel::HandlePlayerAttack()
    {
        if (!player)
        {
            return;
        }

        auto playerGameObject = player->GetGameObject();
        if (!playerGameObject)
        {
            return;
        }

        auto playerInput = playerGameObject->GetComponent<InputComponent>();
        if (!playerInput || !playerInput->IsLeftMousePressed())
        {
            return;
        }

        auto playerCombat = playerGameObject->GetComponent<CombatComponent>();
        if (!playerCombat || !playerCombat->CanAttack())
        {
            return;
        }

        for (auto& enemy : enemies)
        {
            if (!enemy)
            {
                continue;
            }

            auto enemyGameObject = enemy->GetGameObject();
            if (!enemyGameObject)
            {
                continue;
            }

            auto enemyHealth = enemyGameObject->GetComponent<HealthComponent>();
            if (!enemyHealth || !enemyHealth->IsAlive())
            {
                continue;
            }

            if (playerCombat->IsInRange(enemyGameObject))
            {
                playerCombat->Attack(enemyGameObject);
                LOG_INFO("Player attacked enemy, enemy health: " + std::to_string(enemyHealth->GetCurrentHealth()));
                break;
            }
        }
    }

    void DeveloperLevel::HandleEnemyAttacks()
    {
        if (!player)
        {
            return;
        }

        auto playerGameObject = player->GetGameObject();
        if (!playerGameObject)
        {
            return;
        }

        auto playerHealth = playerGameObject->GetComponent<HealthComponent>();
        if (!playerHealth || !playerHealth->IsAlive())
        {
            return;
        }

        for (auto& enemy : enemies)
        {
            if (!enemy)
            {
                continue;
            }

            auto enemyGameObject = enemy->GetGameObject();
            if (!enemyGameObject)
            {
                continue;
            }

            auto enemyHealth = enemyGameObject->GetComponent<HealthComponent>();
            if (!enemyHealth || !enemyHealth->IsAlive())
            {
                continue;
            }

            auto enemyCombat = enemyGameObject->GetComponent<CombatComponent>();
            if (!enemyCombat || !enemyCombat->CanAttack())
            {
                continue;
            }

            if (enemyCombat->IsInRange(playerGameObject))
            {
                enemyCombat->Attack(playerGameObject);
                LOG_INFO("Enemy attacked player, player health: " + std::to_string(playerHealth->GetCurrentHealth()));
            }
        }
    }

    void DeveloperLevel::SpawnEnemies()
    {
        auto enemyPositions = levelGenerator->GetEnemyStarts();

        if (enemyPositions.empty())
        {
            LOG_WARN("No enemy positions found, using default");
            enemyPositions.push_back(enemyStartPosition);
        }

        for (const auto& enemyPos : enemyPositions)
        {
            auto enemy = std::make_shared<Enemy>(
                player->GetGameObject(),
                enemyPos
            );
            enemies.push_back(enemy);
        }

        LOG_INFO("Spawned " + std::to_string(enemies.size()) + " enemies");
    }
}