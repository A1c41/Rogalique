#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Engine.h"
#include "ResourceSystem.h"
#include "DeveloperLevel.h"

using namespace Rogalique;

int main()
{
	GameEngine::RenderSystem::Instance()->SetMainWindow(new sf::RenderWindow(sf::VideoMode(1280, 720), "Rogalique"));
	GameEngine::ResourceSystem::Instance()->LoadTexture("ball", "Resources//ball.png");

	auto developerLevel = std::make_shared<DeveloperLevel>();
	developerLevel->Start();

	GameEngine::Engine::Instance()->Run();

	return 0;
}


//#include "GameState.h"
//#include <cstdlib>
//#include <ctime>
//#include "Engine/Engine.h"
//
//int main() {
//    GameEngine::Engine engine;
//    engine.Initialize();
//    engine.Run();
//
//    srand(static_cast<unsigned>(time(nullptr)));
//    Rogalique::GameState game;
//    sf::Clock clock;
//
//    while (game.isRunning()) {
//        float dt = clock.restart().asSeconds();
//        sf::Event event;
//        while (game.getWindow().pollEvent(event)) {
//            game.handleInput(event);
//        }
//        game.update(dt);
//        game.render();
//    }
//    return 0;
//}