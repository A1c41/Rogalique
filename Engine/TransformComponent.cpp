#include "pch.h"
#include "TransformComponent.h"
#include "GameObject.h"

namespace GameEngine
{
	TransformComponent::TransformComponent(GameObject* gameObject) : Component(gameObject) {}

	void TransformComponent::Update(float deltaTime)
	{

	}

	void TransformComponent::Render()
	{

	}

	void TransformComponent::MoveBy(const Vector2Df& offset)
	{
		localPosition.x += offset.x;
		localPosition.y += offset.y;
		isUpdated = false;
	}

	void TransformComponent::MoveBy(float offsetX, float offsetY)
	{
		localPosition.x += offsetX;
		localPosition.y += offsetY;
		isUpdated = false;
	}

	void TransformComponent::SetWorldPosition(const Vector2Df& position)
	{
		if (parent == nullptr)
		{
			localPosition = position;
		}
		else
		{
			Matrix2D newWorldTransform = createTransform(position, GetWorldRotation(), GetWorldScale());
			localTransform = parent->GetWorldTransform().GetInversed() * newWorldTransform;
			setLocalInfoFrom(localTransform);
		}
		isUpdated = false;
	}

	void TransformComponent::SetWorldPosition(float positionX, float positionY)
	{
		SetWorldPosition(Vector2Df(positionX, positionY));
	}

	void TransformComponent::SetLocalPosition(const Vector2Df& position)
	{
		localPosition = position;
		isUpdated = false;
	}

	void TransformComponent::SetLocalPosition(float positionX, float positionY)
	{
		localPosition.x = positionX;
		localPosition.y = positionY;
		isUpdated = false;
	}

	void TransformComponent::RotateBy(float offset)
	{
		localRotation += offset;
		while (localRotation >= 360) localRotation -= 360;
		while (localRotation < 0) localRotation += 360;
		isUpdated = false;
	}

	void TransformComponent::SetWorldRotation(float angle)
	{
		if (parent == nullptr)
		{
			localRotation = angle;
		}
		else
		{
			Matrix2D newWorldTransform = createTransform(GetWorldPosition(), angle, GetWorldScale());
			localTransform = parent->GetWorldTransform().GetInversed() * newWorldTransform;
			setLocalInfoFrom(localTransform);
		}
		isUpdated = false;
	}

	void TransformComponent::SetLocalRotation(float angle)
	{
		localRotation = angle;
		while (localRotation >= 360) localRotation -= 360;
		while (localRotation < 0) localRotation += 360;
		isUpdated = false;
	}

	void TransformComponent::ScaleBy(const Vector2Df& scale)
	{
		localScale.x *= scale.x;
		localScale.y *= scale.y;
		isUpdated = false;
	}

	void TransformComponent::ScaleBy(float scaleX, float scaleY)
	{
		localScale.x *= scaleX;
		localScale.y *= scaleY;
		isUpdated = false;
	}

	void TransformComponent::SetWorldScale(const Vector2Df& scale)
	{
		if (parent == nullptr)
		{
			localScale = scale;
		}
		else
		{
			Matrix2D newWorldTransform = createTransform(GetWorldPosition(), GetWorldRotation(), scale);
			localTransform = parent->GetWorldTransform().GetInversed() * newWorldTransform;
			setLocalInfoFrom(localTransform);
		}
		isUpdated = false;
	}

	void TransformComponent::SetWorldScale(float scaleX, float scaleY)
	{
		SetWorldScale(Vector2Df(scaleX, scaleY));
	}

	void TransformComponent::SetLocalScale(const Vector2Df& scale)
	{
		localScale = scale;
		isUpdated = false;
	}

	void TransformComponent::SetLocalScale(float scaleX, float scaleY)
	{
		localScale.x = scaleX;
		localScale.y = scaleY;
		isUpdated = false;
	}

	const Vector2Df& TransformComponent::GetWorldPosition() const
	{
		updateLocalTransform();
		if (parent == nullptr)
		{
			position = localPosition;
			return position;
		}

		Matrix2D worldTransform = parent->GetWorldTransform() * localTransform;
		position.x = worldTransform.GetMatrix()[0][2];
		position.y = worldTransform.GetMatrix()[1][2];
		return position;
	}

	const Vector2Df& TransformComponent::GetLocalPosition() const
	{
		updateLocalTransform();
		return localPosition;
	}

	const float TransformComponent::GetWorldRotation() const
	{
		updateLocalTransform();
		if (parent == nullptr)
		{
			return localRotation;
		}

		Matrix2D worldTransform = parent->GetWorldTransform() * localTransform;
		rotation = std::atan2(worldTransform.GetMatrix()[0][1], worldTransform.GetMatrix()[0][0]) * 180 / 3.14159265f;
		return rotation;
	}

	const float TransformComponent::GetLocalRotation() const
	{
		updateLocalTransform();
		return localRotation;
	}

	const Vector2Df& TransformComponent::GetWorldScale() const
	{
		updateLocalTransform();
		if (parent == nullptr)
		{
			scale = localScale;
			return scale;
		}

		Matrix2D worldTransform = parent->GetWorldTransform() * localTransform;
		scale.x = std::sqrt(worldTransform.GetMatrix()[0][0] * worldTransform.GetMatrix()[0][0] + worldTransform.GetMatrix()[1][0] * worldTransform.GetMatrix()[1][0]);
		scale.y = std::sqrt(worldTransform.GetMatrix()[0][1] * worldTransform.GetMatrix()[0][1] + worldTransform.GetMatrix()[1][1] * worldTransform.GetMatrix()[1][1]);
		return scale;
	}

	const Vector2Df& TransformComponent::GetLocalScale() const
	{
		updateLocalTransform();
		return localScale;
	}

	void TransformComponent::SetParent(TransformComponent* newParent)
	{
		if (parent == nullptr && newParent != nullptr)
		{
			newParent->gameObject->AddChild(gameObject);
			localTransform = newParent->GetWorldTransform().GetInversed() * localTransform;
		}
		else if (parent != nullptr && newParent == nullptr)
		{
			parent->gameObject->RemoveChild(gameObject);
			localTransform = parent->GetWorldTransform() * localTransform;
		}
		else if (parent != nullptr && newParent != nullptr)
		{
			parent->gameObject->RemoveChild(gameObject);
			newParent->gameObject->AddChild(gameObject);
			localTransform = newParent->GetWorldTransform().GetInversed() * (parent->GetWorldTransform() * localTransform);
		}

		setLocalInfoFrom(localTransform);
		parent = newParent;
		isUpdated = false;
	}

	TransformComponent* TransformComponent::GetParent() const
	{
		return parent;
	}

	const Matrix2D TransformComponent::GetWorldTransform() const
	{
		updateLocalTransform();

		if (parent == nullptr)
		{
			return localTransform;
		}

		return parent->GetWorldTransform() * localTransform;
	}

	void TransformComponent::Print() const
	{
		std::cout << "Transform name : " << gameObject->GetName() << std::endl;
		std::cout << "Position World: (" << GetWorldPosition().x << ", " << GetWorldPosition().y << ")" << std::endl;
		std::cout << "Position Local: (" << GetLocalPosition().x << ", " << GetLocalPosition().y << ")" << std::endl;
		std::cout << "Rotation World: " << GetWorldRotation() << std::endl;
		std::cout << "Rotation Local: " << GetLocalRotation() << std::endl;
		std::cout << "Scale World: (" << GetWorldScale().x << ", " << GetWorldScale().y << ")" << std::endl;
		std::cout << "Scale Local: (" << GetLocalScale().x << ", " << GetLocalScale().y << ")" << std::endl;
	}

	void TransformComponent::setWorldInfoFrom(const Matrix2D& transform) const
	{
		position.x = transform.GetMatrix()[0][2];
		position.y = transform.GetMatrix()[1][2];
		scale.x = std::sqrt(transform.GetMatrix()[0][0] * transform.GetMatrix()[0][0] + transform.GetMatrix()[1][0] * transform.GetMatrix()[1][0]);
		scale.y = std::sqrt(transform.GetMatrix()[0][1] * transform.GetMatrix()[0][1] + transform.GetMatrix()[1][1] * transform.GetMatrix()[1][1]);
		rotation = std::atan2(transform.GetMatrix()[0][1], transform.GetMatrix()[0][0]) * 180 / 3.14159265f;
	}

	void TransformComponent::setLocalInfoFrom(const Matrix2D& transform) const
	{
		localPosition.x = transform.GetMatrix()[0][2];
		localPosition.y = transform.GetMatrix()[1][2];
		localScale.x = std::sqrt(transform.GetMatrix()[0][0] * transform.GetMatrix()[0][0] + transform.GetMatrix()[1][0] * transform.GetMatrix()[1][0]);
		localScale.y = std::sqrt(transform.GetMatrix()[0][1] * transform.GetMatrix()[0][1] + transform.GetMatrix()[1][1] * transform.GetMatrix()[1][1]);
		localRotation = std::atan2(transform.GetMatrix()[0][1], transform.GetMatrix()[0][0]) * 180 / 3.14159265f;
	}

	void TransformComponent::updateLocalTransform() const
	{
		if (!isUpdated)
		{
			localTransform = createTransform(localPosition, localRotation, localScale);
			isUpdated = true;
		}
	}

	Matrix2D TransformComponent::createTransform(const Vector2Df& position, float rotation, const Vector2Df& scale) const
	{
		return Matrix2D(position, rotation, scale);
	}
}