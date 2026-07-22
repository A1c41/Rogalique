#include "pch.h"
#include "Engine.h"
#include <iostream>
#include "GameWorld.h"
#include "RenderSystem.h"
#include "Logger.h"

namespace GameEngine
{
	Engine* Engine::Instance()
	{
		static Engine instance;
		return &instance;
	}

	Engine::Engine()
	{
		unsigned int seed = (unsigned int)time(nullptr);
		srand(seed);
		LOG_INFO("Engine initialized with seed: " + std::to_string(seed));
	}

	void Engine::Run()
	{
		sf::Clock gameClock;
		sf::Event event;

		LOG_INFO("Engine main loop started");

		if (!RenderSystem::Instance()->GetMainWindow().isOpen()) {
			LOG_ERROR("Main window is not open at start of Run()");
			throw std::runtime_error("Main window is not open");
		}

		while (RenderSystem::Instance()->GetMainWindow().isOpen())
		{
			sf::Time dt = gameClock.restart();
			float fps = dt.asSeconds();

			while (RenderSystem::Instance()->GetMainWindow().pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					LOG_INFO("Window close event received");
					RenderSystem::Instance()->GetMainWindow().close();
				}
			}

			if (!RenderSystem::Instance()->GetMainWindow().isOpen())
			{
				LOG_INFO("Window closed, exiting main loop");
				break;
			}

			RenderSystem::Instance()->GetMainWindow().clear();

			GameWorld::Instance()->Update(fps);
			GameWorld::Instance()->FixedUpdate(fps);
			GameWorld::Instance()->Render();
			GameWorld::Instance()->LateUpdate();

			RenderSystem::Instance()->GetMainWindow().display();
		}

		LOG_INFO("Engine main loop ended");
	}
}