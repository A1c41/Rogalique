#pragma once

#include "framework.h"

namespace GameEngine
{
	class ColliderComponent;
	struct ENGINE_API Trigger
	{
	public:
		Trigger(ColliderComponent* newFirst, ColliderComponent* newSecond) :
			first(newFirst), second(newSecond) {
		};
	private:
		ColliderComponent* first;
		ColliderComponent* second;
	};
}