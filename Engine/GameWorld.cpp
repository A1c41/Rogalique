#include "pch.h"
#include "GameWorld.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "PhysicsSystem.h"
#include "ColliderComponent.h"
#include "Logger.h"

namespace GameEngine
{
	GameWorld* GameWorld::Instance()
	{
		static GameWorld world;
		return &world;
	}

	void GameWorld::Update(float fps)
	{
		for (int i = 0; i < gameObjects.size(); i++)
		{
			if (gameObjects[i])
			{
				gameObjects[i]->Update(fps);
			}
		}
	}

	void GameWorld::FixedUpdate(float fps)
	{
		fixedCounter += fps;
		if (fixedCounter > PhysicsSystem::Instance()->GetFixedDeltaTime())
		{
			fixedCounter -= PhysicsSystem::Instance()->GetFixedDeltaTime();
			PhysicsSystem::Instance()->Update();
		}
	}

	void GameWorld::Render()
	{
		for (int i = 0; i < gameObjects.size(); i++)
		{
			if (gameObjects[i])
			{
				gameObjects[i]->Render();
			}
		}
	}

	void GameWorld::LateUpdate()
	{
		for (int i = markedToDestroyGameObjects.size() - 1; i >= 0; i--)
		{
			DestroyGameObjectImmediate(markedToDestroyGameObjects[i]);
		}
	}

	GameObject* GameWorld::CreateGameObject()
	{
		GameObject* newGameObject = new GameObject();
		if (!newGameObject) {
			LOG_ERROR("Failed to create new GameObject");
			throw std::bad_alloc();
		}
		gameObjects.push_back(newGameObject);
		LOG_INFO("Created GameObject: " + newGameObject->GetName());
		return newGameObject;
	}

	GameObject* GameWorld::CreateGameObject(std::string name)
	{
		GameObject* newGameObject = new GameObject(name);
		if (!newGameObject) {
			LOG_ERROR("Failed to create GameObject: " + name);
			throw std::bad_alloc();
		}
		gameObjects.push_back(newGameObject);
		LOG_INFO("Created GameObject: " + name);
		return newGameObject;
	}

	void GameWorld::DestroyGameObject(GameObject* gameObject)
	{
		if (gameObject)
		{
			LOG_INFO("Marking GameObject for destruction: " + gameObject->GetName());
			markedToDestroyGameObjects.push_back(gameObject);
		}
		else
		{
			LOG_WARN("Attempted to destroy null GameObject");
		}
	}

	void GameWorld::Clear()
	{
		LOG_INFO("Clearing GameWorld, objects count: " + std::to_string(gameObjects.size()));

		for (int i = gameObjects.size() - 1; i >= 0; i--)
		{
			if (gameObjects[i] == nullptr)
			{
				continue;
			}

			if (gameObjects[i]->GetComponent<TransformComponent>()->GetParent() == nullptr)
			{
				DestroyGameObjectImmediate(gameObjects[i]);
			}
		}

		fixedCounter = 0.f;
		LOG_INFO("GameWorld cleared");
	}

	void GameWorld::Print() const
	{
		for (auto& obj : gameObjects)
		{
			if (obj == nullptr)
			{
				continue;
			}
			if (obj->GetComponent<TransformComponent>()->GetParent() == nullptr)
			{
				obj->Print();
			}
		}
	}

	void GameWorld::DestroyGameObjectImmediate(GameObject* gameObject)
	{
		if (!gameObject)
		{
			LOG_WARN("Attempted to immediately destroy null GameObject");
			return;
		}

		std::string objName = gameObject->GetName();
		LOG_INFO("Immediately destroying GameObject: " + objName);

		auto parent = gameObject->GetComponent<TransformComponent>()->GetParent();
		if (parent != nullptr)
		{
			parent->GetGameObject()->RemoveChild(gameObject);
		}

		for (auto transform : gameObject->GetComponentsInChildren<TransformComponent>())
		{
			GameObject* gameObjectToDelete = transform->GetGameObject();
			ASSERT_MSG(gameObjectToDelete != nullptr, "Transform component has no owner GameObject");

			gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(),
				[gameObjectToDelete](GameObject* obj) { return obj == gameObjectToDelete; }),
				gameObjects.end());
			markedToDestroyGameObjects.erase(std::remove_if(markedToDestroyGameObjects.begin(), markedToDestroyGameObjects.end(),
				[gameObjectToDelete](GameObject* obj) { return obj == gameObjectToDelete; }),
				markedToDestroyGameObjects.end());

			delete gameObjectToDelete;
		}
	}
}