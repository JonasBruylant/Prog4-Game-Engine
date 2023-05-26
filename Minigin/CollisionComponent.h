#pragma once
#include "Component.h"

namespace dae
{
	struct Box
	{
		float xPos, yPos;
		int width, height;
	};


class CollisionComponent : public Component
{
public:

	CollisionComponent(std::weak_ptr<GameObject> pOwner);
	~CollisionComponent() = default;

	CollisionComponent(const CollisionComponent& other) = delete;
	CollisionComponent(CollisionComponent&& other) = delete;
	CollisionComponent& operator=(const CollisionComponent& other) = delete;
	CollisionComponent& operator=(CollisionComponent&& other) = delete;

protected:
	void Update() override;
private:
	Box m_collisionBox;
	//Either have the component and move along with it
	//Or have the collision component be a child of the pOwner
};

}
