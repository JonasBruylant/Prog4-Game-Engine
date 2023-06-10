#include "CollisionComponent.h"
#include "Renderer.h"
#include "Transform.h"
#include "Scene.h"

dae::CollisionComponent::CollisionComponent(GameObject* pOwner):
	Component(pOwner)
{
	
	m_pOwnerTransform = pOwner->GetTransform();
	m_pCurrentScene = pOwner->GetScene();

}

bool dae::CollisionComponent::IsBoxOverlapping(float xPos, float yPos, Box otherBox)
{
	auto& ownerPos = m_pOwnerTransform->GetWorldPosition();

	if (ownerPos.x > xPos + otherBox.width || xPos > ownerPos.x + m_collisionBox.width) return false;
	
	if (ownerPos.y > yPos + otherBox.height || yPos > ownerPos.y + m_collisionBox.height) return false;

	return true;
}

bool dae::CollisionComponent::IsBoxOverlapping(GameObject* otherGameObject)
{
	auto transform = otherGameObject->GetComponent<TransformComponent>();

	if (!transform)
		return false;
	auto& worldPosition = transform->GetWorldPosition();

	return IsBoxOverlapping(worldPosition.x, worldPosition.y, otherGameObject->GetComponent<CollisionComponent>()->GetMeasurements());
}


void dae::CollisionComponent::Update()
{
	//Calling GetworldPosition so transform dirtyflag gets cleared.
	m_pOwnerTransform->GetWorldPosition();
	auto objectsCollidingWith = m_pCurrentScene->HandleCollision(GetOwner());

	if (objectsCollidingWith.empty())
		return;

	for (auto& currentCollision : objectsCollidingWith)
	{
		if(m_onTriggerCallback)
			m_onTriggerCallback(currentCollision->GetComponent<CollisionComponent>().get(), currentCollision);
	}
}

void dae::CollisionComponent::Render() const
{

#if _DEBUG
	if (m_DrawDebug)
	{
		auto& ownerPos = m_pOwnerTransform->GetWorldPosition();
		Renderer::GetInstance().RenderCollisionRectangle(ownerPos.x + m_collisionBox.xOffset, ownerPos.y + m_collisionBox.yOffset, m_collisionBox.width, m_collisionBox.height, m_DebugRectColor.r, m_DebugRectColor.g, m_DebugRectColor.b);

	}
#endif
}
