#pragma once

#include <vector>
#include <string>
#include <memory>
#include "GameObject.h"
#include "Vector.h"

namespace Rogalique
{
    struct LevelData
    {
        std::vector<std::string> grid;
        GameEngine::Vector2Df playerStart;
        std::vector<GameEngine::Vector2Df> enemyStarts;
        int tileSize = 32;
        int width = 0;
        int height = 0;
    };

    class LevelLoader
    {
    public:
        LevelLoader();
        ~LevelLoader();

        void LoadAllLevels(const std::string& folderPath);
        LevelData GetRandomLevel() const;
        LevelData GetLevel(int index) const;
        int GetLevelCount() const;

    private:
        std::vector<LevelData> levels;
        bool LoadLevelFromFile(const std::string& filePath, LevelData& outData);
        void ParseLevelGrid(LevelData& data);
    };
}