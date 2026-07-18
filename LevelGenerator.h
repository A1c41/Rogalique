#pragma once

#include <vector>
#include <memory>
#include "Wall.h"
#include "Floor.h"

namespace Rogalique
{
    class LevelGenerator
    {
    public:
        LevelGenerator();
        ~LevelGenerator();

        void GenerateLevel(const GameEngine::Vector2Df& center,
            int widthInTiles,
            int heightInTiles,
            int tileSize);

        void Clear();

    private:
        std::vector<std::unique_ptr<Wall>> walls;
        std::vector<std::unique_ptr<Floor>> floors;

        bool IsOnBorder(int x, int y, int width, int height) const;
    };
}