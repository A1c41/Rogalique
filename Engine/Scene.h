#pragma once

#include "framework.h"

namespace GameEngine
{
	class ENGINE_API Scene
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void Start() = 0;
		virtual void Restart() = 0;
		virtual void Stop() = 0;
	};
}