#pragma once

#include "framework.h"
#include <iostream>
#include <array>
#include <memory>
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "LevelGenerator.h"

using namespace GameEngine;

namespace Rogalique
{
    class DeveloperLevel : public Scene
    {
    public:
        DeveloperLevel();
        void Start() override;
        void Restart() override;
        void Stop() override;

    private:
        std::shared_ptr<Player> player;
        std::shared_ptr<Enemy> enemy;
        std::shared_ptr<LevelGenerator> levelGenerator;

        Vector2Df playerStartPosition = { 400, 360 };
        Vector2Df enemyStartPosition = { 700, 360 };
    };
}