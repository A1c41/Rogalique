#pragma once

#include "framework.h"
#include <iostream>
#include <array>
#include "Scene.h"
#include "Player.h"

using namespace GameEngine;

namespace Rogalique
{
	class DeveloperLevel : public Scene
	{
	public:
		void Start() override;
		void Restart() override;
		void Stop() override;

	private:
		std::shared_ptr<Player> player;
	};
}