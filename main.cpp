#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Engine.h"
#include "ResourceSystem.h"
#include "SoundSystem.h"
#include "DeveloperLevel.h"
#include "Matrix2D.h"
#include "Logger.h"
#include "GameWorld.h"

using namespace Rogalique;

int main()
{
    try {
        LOG_INFO("Application starting");

        auto logger = std::make_shared<Logger>();
        logger->addSink(std::make_shared<ConsoleSink>());
        try {
            logger->addSink(std::make_shared<FileSink>("rogalique.log"));
        }
        catch (const std::exception& e) {
            logger->warn("Failed to create file sink: " + std::string(e.what()));
        }
        LoggerRegistry::getInstance().registerLogger("global", logger);

        LOG_INFO("Setting up main window");
        GameEngine::RenderSystem::Instance()->SetMainWindow(new sf::RenderWindow(sf::VideoMode(1280, 720), "Rogalique"));

        LOG_INFO("Loading textures");
        GameEngine::ResourceSystem::Instance()->LoadTexture("hero", "Resources//hero.png");
        GameEngine::ResourceSystem::Instance()->LoadTexture("enemy", "Resources//enemy.png");
        GameEngine::ResourceSystem::Instance()->LoadTexture("wall", "Resources//wall.png");
        GameEngine::ResourceSystem::Instance()->LoadTexture("floor", "Resources//floor.png");

        LOG_INFO("Creating developer level");
        auto developerLevel = std::make_shared<DeveloperLevel>();
        developerLevel->Start();

        LOG_INFO("Starting game loop");
        sf::Clock gameClock;
        sf::Event event;

        while (GameEngine::RenderSystem::Instance()->GetMainWindow().isOpen())
        {
            sf::Time dt = gameClock.restart();
            float fps = dt.asSeconds();

            while (GameEngine::RenderSystem::Instance()->GetMainWindow().pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    GameEngine::RenderSystem::Instance()->GetMainWindow().close();
                }
            }

            if (!GameEngine::RenderSystem::Instance()->GetMainWindow().isOpen())
            {
                break;
            }

            GameEngine::RenderSystem::Instance()->GetMainWindow().clear();

            developerLevel->Update(fps);
            GameEngine::GameWorld::Instance()->Update(fps);
            GameEngine::GameWorld::Instance()->FixedUpdate(fps);
            GameEngine::GameWorld::Instance()->Render();
            GameEngine::GameWorld::Instance()->LateUpdate();

            GameEngine::RenderSystem::Instance()->GetMainWindow().display();
        }

        LOG_INFO("Application closing normally");
        return 0;
    }
    catch (const std::exception& e) {
        LOG_ERROR("Fatal error: " + std::string(e.what()));
        return 1;
    }
    catch (...) {
        LOG_ERROR("Unknown fatal error occurred");
        return 1;
    }
}