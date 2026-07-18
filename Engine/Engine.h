#pragma once
#define NOMINMAX

#include "framework.h"
#include "SFML/Graphics.hpp"

namespace GameEngine
{
	class ENGINE_API Engine
	{
	public:
		Engine(const Engine& app) = delete;
		Engine& operator= (const Engine&) = delete;

		static Engine* Instance();

		void Run();

	private:
		Engine();
		~Engine() = default;
	};
}
