#include "DeveloperLevel.h"
#include "GameWorld.h"
#include "GameObject.h"

using namespace GameEngine;

namespace Rogalique
{
	void DeveloperLevel::Start()
	{
		player = std::make_shared<Player>();
	}

	void DeveloperLevel::Restart()
	{
		Stop();
		Start();
	}

	void DeveloperLevel::Stop()
	{
		GameWorld::Instance()->Clear();
	}
}