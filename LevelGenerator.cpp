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

    void LevelGenerator::LoadLevels(const std::string& folderPath)
    {
        levelLoader.LoadAllLevels(folderPath);
    }

    LevelData LevelGenerator::GetRandomLevel() const
    {
        return levelLoader.GetRandomLevel();
    }

    bool LevelGenerator::GenerateLevelFromFile(const LevelData& levelData)
    {
        Clear();

        if (levelData.grid.empty())
        {
            std::cout << "Level data is empty!" << std::endl;
            return false;
        }

        int tileSize = levelData.tileSize;
        int width = levelData.width;
        int height = levelData.height;

        float offsetX = -width * tileSize / 2.0f;
        float offsetY = -height * tileSize / 2.0f;

        playerStart = {
            levelData.playerStart.x + offsetX,
            levelData.playerStart.y + offsetY
        };

        enemyStarts.clear();
        for (const auto& enemyPos : levelData.enemyStarts)
        {
            enemyStarts.push_back({
                enemyPos.x + offsetX,
                enemyPos.y + offsetY
                });
        }

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                char c = levelData.grid[y][x];

                float posX = x * tileSize + tileSize / 2.0f + offsetX;
                float posY = y * tileSize + tileSize / 2.0f + offsetY;
                GameEngine::Vector2Df position(posX, posY);

                if (c == '#')
                {
                    walls.push_back(std::make_unique<Wall>(
                        position,
                        GameEngine::Vector2Df(static_cast<float>(tileSize), static_cast<float>(tileSize))
                    ));
                }
                else if (c == '.')
                {
                    floors.push_back(std::make_unique<Floor>(position, tileSize));
                }
            }
        }

        std::cout << "Level generated: " << walls.size() << " walls, " << floors.size() << " floors" << std::endl;
        std::cout << "Player start: (" << playerStart.x << ", " << playerStart.y << ")" << std::endl;
        std::cout << "Enemy count: " << enemyStarts.size() << std::endl;

        return true;
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

        playerStart = center;
        enemyStarts.clear();
        enemyStarts.push_back({ center.x + 300, center.y });
    }

    bool LevelGenerator::IsOnBorder(int x, int y, int width, int height) const
    {
        return x == 0 || x == width - 1 || y == 0 || y == height - 1;
    }

    void LevelGenerator::Clear()
    {
        walls.clear();
        floors.clear();
        playerStart = { 0, 0 };
        enemyStarts.clear();
    }
}