#pragma once

#include <vector>
#include <memory>
#include "Wall.h"
#include "Floor.h"
#include "LevelLoader.h"

namespace Rogalique
{
    class LevelGenerator
    {
    public:
        LevelGenerator();
        ~LevelGenerator();

        void LoadLevels(const std::string& folderPath);
        bool GenerateLevelFromFile(const LevelData& levelData);
        LevelData GetRandomLevel() const;

        void GenerateLevel(const GameEngine::Vector2Df& center,
            int widthInTiles,
            int heightInTiles,
            int tileSize);

        void Clear();

        GameEngine::Vector2Df GetPlayerStart() const { return playerStart; }
        std::vector<GameEngine::Vector2Df> GetEnemyStarts() const { return enemyStarts; }

    private:
        std::vector<std::unique_ptr<Wall>> walls;
        std::vector<std::unique_ptr<Floor>> floors;

        LevelLoader levelLoader;
        GameEngine::Vector2Df playerStart;
        std::vector<GameEngine::Vector2Df> enemyStarts;

        bool IsOnBorder(int x, int y, int width, int height) const;
    };
}