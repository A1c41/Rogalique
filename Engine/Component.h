#pragma once
#include "framework.h"

namespace GameEngine
{
	class GameObject;

	class ENGINE_API Component
	{
	public:
		Component(GameObject* gameObject);
		virtual ~Component();

		virtual void Update(float fps) = 0;
		virtual void Render() = 0;

		GameObject* GetGameObject();

	protected:
		GameObject* gameObject;
	};
}