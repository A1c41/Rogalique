#include "DeveloperLevel.h"
#include "GameWorld.h"
#include "GameObject.h"
#include "SoundSystem.h"
#include "ResourceSystem.h"

using namespace GameEngine;

namespace Rogalique
{
    DeveloperLevel::DeveloperLevel()
    {
        levelGenerator = std::make_shared<LevelGenerator>();
        levelGenerator->LoadLevels("Resources/Levels");
    }

    void DeveloperLevel::Start()
    {
        SoundSystem::Instance()->LoadMusic("background", "Resources//background_music.ogg");
        SoundSystem::Instance()->PlayMusic("background", true);
        SoundSystem::Instance()->SetMusicVolume("background", 70.0f);

        LevelData levelData = levelGenerator->GetRandomLevel();

        if (!levelGenerator->GenerateLevelFromFile(levelData))
        {
            levelGenerator->GenerateLevel(playerStartPosition, 25, 15, 32);
        }

        Vector2Df playerPos = levelGenerator->GetPlayerStart();
        if (playerPos.x == 0 && playerPos.y == 0)
        {
            playerPos = playerStartPosition;
        }

        player = std::make_shared<Player>();
        auto playerTransform = player->GetGameObject()->GetComponent<TransformComponent>();
        if (playerTransform)
        {
            playerTransform->SetWorldPosition(playerPos);
        }

        auto enemyPositions = levelGenerator->GetEnemyStarts();

        if (enemyPositions.empty())
        {
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

        std::cout << "Created " << enemies.size() << " enemies" << std::endl;
    }

    void DeveloperLevel::Restart()
    {
        Stop();
        Start();
    }

    void DeveloperLevel::Stop()
    {
        SoundSystem::Instance()->StopMusic("background");

        GameWorld::Instance()->Clear();

        player.reset();
        enemies.clear();
        levelGenerator->Clear();
    }
}