#include "LevelGenerator.h"

namespace Rogalique
{
    LevelGenerator::LevelGenerator()
    {
    }

    LevelGenerator::~LevelGenerator()
    {
        Clear();
    }

    void LevelGenerator::GenerateLevel(const GameEngine::Vector2Df& center,
        int widthInTiles,
        int heightInTiles,
        int tileSize)
    {
        Clear();

        float startX = center.x - (widthInTiles * tileSize) / 2.0f;
        float startY = center.y - (heightInTiles * tileSize) / 2.0f;

        for (int y = 0; y < heightInTiles; y++)
        {
            for (int x = 0; x < widthInTiles; x++)
            {
                float posX = startX + x * tileSize + tileSize / 2.0f;
                float posY = startY + y * tileSize + tileSize / 2.0f;
                GameEngine::Vector2Df position(posX, posY);

                if (IsOnBorder(x, y, widthInTiles, heightInTiles))
                {
                    walls.push_back(std::make_unique<Wall>(
                        position,
                        GameEngine::Vector2Df(static_cast<float>(tileSize), static_cast<float>(tileSize))
                    ));
                }
                else
                {
                    floors.push_back(std::make_unique<Floor>(position, tileSize));
                }
            }
        }
    }

    bool LevelGenerator::IsOnBorder(int x, int y, int width, int height) const
    {
        return x == 0 || x == width - 1 || y == 0 || y == height - 1;
    }

    void LevelGenerator::Clear()
    {
        walls.clear();
        floors.clear();
    }
}