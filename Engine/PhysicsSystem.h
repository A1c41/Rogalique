#pragma once

#include "framework.h"
#include <vector>
#include <utility>
#include <iostream>
#include "ColliderComponent.h"
#include "RigidbodyComponent.h"
#include "Vector.h"

namespace GameEngine
{
	class ENGINE_API PhysicsSystem
	{
	public:
		static PhysicsSystem* Instance();

		void Update();

		float GetFixedDeltaTime() const;
		void Subscribe(ColliderComponent* collider);
		void Unsubscribe(ColliderComponent* collider);
	private:
		PhysicsSystem() {}
		~PhysicsSystem() {}

		PhysicsSystem(PhysicsSystem const&) = delete;
		PhysicsSystem& operator= (PhysicsSystem const&) = delete;

		std::vector<ColliderComponent*> colliders;
		std::vector<std::pair<ColliderComponent*, ColliderComponent*>> triggersEnteredPair;

		float fixedDeltaTime = 0.02f;
	};
}