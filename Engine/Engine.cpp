#include "pch.h"
#include "Engine.h"
#include <iostream>

namespace GameEngine
{
	Engine::Engine()
	{

	}

	void Engine::Initialize()
	{
		std::cout << "Engine init!" << std::endl;
	}

	void Engine::Run()
	{
		std::cout << "Engin run!" << std::endl;
	}
}