#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Engine.h"
#include "ResourceSystem.h"
#include "SoundSystem.h"
#include "DeveloperLevel.h"
#include "Matrix2D.h"

using namespace Rogalique;

int main()
{
    GameEngine::RenderSystem::Instance()->SetMainWindow(new sf::RenderWindow(sf::VideoMode(1280, 720), "Rogalique"));

    GameEngine::ResourceSystem::Instance()->LoadTexture("hero", "Resources//hero.png");
    GameEngine::ResourceSystem::Instance()->LoadTexture("enemy", "Resources//enemy.png");
    GameEngine::ResourceSystem::Instance()->LoadTexture("wall", "Resources//wall.png");
    GameEngine::ResourceSystem::Instance()->LoadTexture("floor", "Resources//floor.png");

    auto developerLevel = std::make_shared<DeveloperLevel>();
    developerLevel->Start();

    GameEngine::Engine::Instance()->Run();

    return 0;
}