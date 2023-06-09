#pragma once
#include "Component.h"
#include "Subject.h"
#include "Transform.h"
#include <functional>


namespace dae
{
	struct Box
	{
		float xOffset{  }, yOffset{  }, width{  }, height{  };
	};
	//class TransformComponent;;
	class Scene;

class CollisionComponent final : public Component
{
public:

	CollisionComponent(std::weak_ptr<GameObject> pOwner);
	~CollisionComponent() = default;

	CollisionComponent(const CollisionComponent& other) = delete;
	CollisionComponent(CollisionComponent&& other) = delete;
	CollisionComponent& operator=(const CollisionComponent& other) = delete;
	CollisionComponent& operator=(CollisionComponent&& other) = delete;

	void SetTag(std::string tag) { m_tag = tag; }
	std::string GetTag() { return m_tag; }

	void SetMeasurements(Box measurements) { m_collisionBox = measurements; }
	void SetMeasurements(float width, float height) { 
		m_collisionBox.xOffset = 0.f;
		m_collisionBox.yOffset = 0.f;
		m_collisionBox.width = width;
		m_collisionBox.height = height;
	}
	Box GetMeasurements() { return m_collisionBox; }

	bool IsBoxOverlapping(float xPos, float yPos, Box otherBox);
	bool IsBoxOverlapping(GameObject* otherGameObject);

	void SetDebugColor(glm::vec3 color) { m_DebugRectColor = color; }
	void SetDrawDebug(bool drawDebug) { m_DrawDebug = drawDebug; }
	void SetOnTriggerEvent(std::function<void(CollisionComponent* otherCollision, GameObject* )> eventTrigger) { m_onTriggerCallback = eventTrigger; }
protected:
	void Update() override;
	void Render() const override;
private:
	Box m_collisionBox;
	std::string m_tag;

	std::function<void(CollisionComponent* otherCollision, GameObject*)> m_onTriggerCallback{};
	std::weak_ptr<TransformComponent> m_pOwnerTransform;

	Scene* m_pCurrentScene;
	glm::vec3 m_DebugRectColor;

	bool m_DrawDebug{ false };
};

}
