#include "pch.h"
#include "Engine.h"
#include <iostream>

Engine::Engine()
{

}

void Engine::Initialize()
{
	std::cout << "Движок инициализирован" << std::endl;
}

void Engine::Run()
{
	std::cout << "Движок запущен!" << std::endl;
}