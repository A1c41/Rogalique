#include "LevelLoader.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <random>
#include <algorithm>
#include "Logger.h"

namespace Rogalique
{
    LevelLoader::LevelLoader()
    {
    }

    LevelLoader::~LevelLoader()
    {
    }

    void LevelLoader::LoadAllLevels(const std::string& folderPath)
    {
        levels.clear();
        LOG_INFO("Loading levels from folder: " + folderPath);

        namespace fs = std::filesystem;

        try
        {
            if (!fs::exists(folderPath)) {
                LOG_ERROR("Level folder does not exist: " + folderPath);
                throw std::runtime_error("Level folder does not exist: " + folderPath);
            }

            for (const auto& entry : fs::directory_iterator(folderPath))
            {
                if (entry.is_regular_file())
                {
                    std::string filePath = entry.path().string();
                    if (filePath.size() >= 4 && filePath.substr(filePath.size() - 4) == ".txt")
                    {
                        LevelData level;
                        if (LoadLevelFromFile(filePath, level))
                        {
                            levels.push_back(level);
                            LOG_INFO("Loaded level: " + entry.path().filename().string());
                        }
                        else
                        {
                            LOG_WARN("Failed to load level from file: " + filePath);
                        }
                    }
                }
            }
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("Error loading levels: " + std::string(e.what()));
        }

        LOG_INFO("Total levels loaded: " + std::to_string(levels.size()));
    }

    bool LevelLoader::LoadLevelFromFile(const std::string& filePath, LevelData& outData)
    {
        LOG_INFO("Loading level from file: " + filePath);

        std::ifstream file(filePath);
        if (!file.is_open())
        {
            LOG_ERROR("Failed to open file: " + filePath);
            return false;
        }

        outData.grid.clear();
        outData.playerStart = { 0, 0 };
        outData.enemyStarts.clear();

        std::string line;
        int row = 0;

        while (std::getline(file, line))
        {
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

            if (line.empty())
                continue;

            outData.grid.push_back(line);
            row++;
        }

        file.close();

        if (outData.grid.empty())
        {
            LOG_ERROR("Empty level file: " + filePath);
            return false;
        }

        outData.height = static_cast<int>(outData.grid.size());
        outData.width = static_cast<int>(outData.grid[0].size());
        outData.tileSize = 32;

        ParseLevelGrid(outData);

        LOG_INFO("Level loaded: " + std::to_string(outData.width) + "x" + std::to_string(outData.height));
        return true;
    }

    void LevelLoader::ParseLevelGrid(LevelData& data)
    {
        data.enemyStarts.clear();

        for (int y = 0; y < data.height; y++)
        {
            if (y >= static_cast<int>(data.grid.size())) {
                LOG_ERROR("Grid height mismatch at row " + std::to_string(y));
                break;
            }

            for (int x = 0; x < data.width; x++)
            {
                if (x >= static_cast<int>(data.grid[y].size())) {
                    LOG_ERROR("Grid width mismatch at position (" + std::to_string(x) + ", " + std::to_string(y) + ")");
                    break;
                }

                char c = data.grid[y][x];

                float posX = x * data.tileSize + data.tileSize / 2.0f;
                float posY = y * data.tileSize + data.tileSize / 2.0f;

                if (c == 'P')
                {
                    data.playerStart = { posX, posY };
                    data.grid[y][x] = '.';
                    LOG_INFO("Player start found at: (" + std::to_string(posX) + ", " + std::to_string(posY) + ")");
                }
                else if (c == 'E')
                {
                    data.enemyStarts.push_back({ posX, posY });
                    data.grid[y][x] = '.';
                    LOG_INFO("Enemy start found at: (" + std::to_string(posX) + ", " + std::to_string(posY) + ")");
                }
            }
        }
    }

    LevelData LevelLoader::GetRandomLevel() const
    {
        if (levels.empty())
        {
            LOG_WARN("No levels loaded, using hardcoded default level");
            LevelData empty;
            empty.grid = {
                "################################",
                "#..............................#",
                "#..........E...................#",
                "#.......#......................#",
                "#.......#......................#",
                "#.......#......................#",
                "#.......#...P..................#",
                "#.......#......................#",
                "#.......#......................#",
                "#.......#......................#",
                "#.......#......................#",
                "##########..####################",
                "#..............................#",
                "#..........#...................#",
                "#..........#...................#",
                "#..........#...................#",
                "#..........#...#...............#",
                "#..........#...#...............#",
                "#..........#...#...............#",
                "#.......E..#...#...............#",
                "#..........#...#################",
                "#..........#........E..........#",
                "#..............................#",
                "################################"
            };
            empty.width = 32;
            empty.height = 24;
            empty.tileSize = 32;
            empty.playerStart = { 7 * 32 + 16, 6 * 32 + 16 };
            empty.enemyStarts.clear();
            empty.enemyStarts.push_back({ 10 * 32 + 16, 2 * 32 + 16 });
            empty.enemyStarts.push_back({ 7 * 32 + 16, 20 * 32 + 16 });
            empty.enemyStarts.push_back({ 21 * 32 + 16, 21 * 32 + 16 });
            return empty;
        }

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, static_cast<int>(levels.size()) - 1);

        return levels[dis(gen)];
    }

    LevelData LevelLoader::GetLevel(int index) const
    {
        if (index >= 0 && index < static_cast<int>(levels.size()))
        {
            return levels[index];
        }
        LOG_ERROR("Level index out of bounds: " + std::to_string(index) + " (total: " + std::to_string(levels.size()) + ")");
        return LevelData();
    }

    int LevelLoader::GetLevelCount() const
    {
        return static_cast<int>(levels.size());
    }
}