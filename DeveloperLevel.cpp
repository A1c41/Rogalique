#include "DeveloperLevel.h"
#include "GameWorld.h"
#include "GameObject.h"
#include "SoundSystem.h"
#include "ResourceSystem.h"
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
        else
        {
            LOG_ERROR("Player transform component not found");
            throw std::runtime_error("Player transform component not found");
        }

        auto enemyPositions = levelGenerator->GetEnemyStarts();

        if (enemyPositions.empty())
        {
            LOG_WARN("No enemy positions found, using default");
            enemyPositions.push_back(enemyStartPosition);
        }

        enemies.clear();
        for (const auto& enemyPos : enemyPositions)
        {
            auto enemy = std::make_shared<Enemy>(
                player->GetGameObject(),
                enemyPos
            );
            enemies.push_back(enemy);
        }

        LOG_INFO("Created " + std::to_string(enemies.size()) + " enemies");
        ASSERT_MSG(!enemies.empty(), "No enemies created for level");
    }

    void DeveloperLevel::Restart()
    {
        LOG_INFO("Restarting developer level");
        Stop();
        Start();
    }

    void DeveloperLevel::Stop()
    {
        LOG_INFO("Stopping developer level");
        SoundSystem::Instance()->StopMusic("background");

        GameWorld::Instance()->Clear();

        player.reset();
        enemies.clear();
        levelGenerator->Clear();

        LOG_INFO("Developer level stopped");
    }
}