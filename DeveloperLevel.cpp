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
    }

    void DeveloperLevel::Start()
    {
        SoundSystem::Instance()->LoadMusic("background", "Resources//background_music.ogg");
        SoundSystem::Instance()->PlayMusic("background", true);
        SoundSystem::Instance()->SetMusicVolume("background", 70.0f);

        levelGenerator->GenerateLevel(playerStartPosition, 25, 15, 32);

        player = std::make_shared<Player>();
        auto playerTransform = player->GetGameObject()->GetComponent<TransformComponent>();
        if (playerTransform)
        {
            playerTransform->SetWorldPosition(playerStartPosition);
        }

        enemy = std::make_shared<Enemy>(
            player->GetGameObject(),
            enemyStartPosition
        );
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
        enemy.reset();
        levelGenerator->Clear();
    }
}