#include "LevelLoader.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <random>
#include <algorithm>

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

        namespace fs = std::filesystem;

        try
        {
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
                            std::cout << "Loaded level: " << entry.path().filename().string() << std::endl;
                        }
                    }
                }
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Error loading levels: " << e.what() << std::endl;
        }

        std::cout << "Total levels loaded: " << levels.size() << std::endl;
    }

    bool LevelLoader::LoadLevelFromFile(const std::string& filePath, LevelData& outData)
    {
        std::ifstream file(filePath);
        if (!file.is_open())
        {
            std::cout << "Failed to open file: " << filePath << std::endl;
            return false;
        }

        outData.grid.clear();
        outData.playerStart = { 0, 0 };
        outData.enemyStarts.clear();  // Изменено

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
            std::cout << "Empty level file: " << filePath << std::endl;
            return false;
        }

        outData.height = outData.grid.size();
        outData.width = outData.grid[0].size();
        outData.tileSize = 32;

        ParseLevelGrid(outData);

        return true;
    }

    void LevelLoader::ParseLevelGrid(LevelData& data)
    {
        data.enemyStarts.clear();

        for (int y = 0; y < data.height; y++)
        {
            for (int x = 0; x < data.width; x++)
            {
                char c = data.grid[y][x];

                float posX = x * data.tileSize + data.tileSize / 2.0f;
                float posY = y * data.tileSize + data.tileSize / 2.0f;

                if (c == 'P')
                {
                    data.playerStart = { posX, posY };
                    data.grid[y][x] = '.';
                }
                else if (c == 'E')
                {
                    data.enemyStarts.push_back({ posX, posY });
                    data.grid[y][x] = '.';
                }
            }
        }
    }

    LevelData LevelLoader::GetRandomLevel() const
    {
        if (levels.empty())
        {
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
                "##########..#################",
                "#..............................#",
                "#..........#...................#",
                "#..........#...................#",
                "#..........#...................#",
                "#..........#...#...............#",
                "#..........#...#...............#",
                "#..........#...#...............#",
                "#.......E..#...#...............#",
                "#..........#...#################",
                "#..........#........E.........#",
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
        std::uniform_int_distribution<> dis(0, levels.size() - 1);

        return levels[dis(gen)];
    }

    LevelData LevelLoader::GetLevel(int index) const
    {
        if (index >= 0 && index < static_cast<int>(levels.size()))
        {
            return levels[index];
        }
        return LevelData();
    }

    int LevelLoader::GetLevelCount() const
    {
        return static_cast<int>(levels.size());
    }
}