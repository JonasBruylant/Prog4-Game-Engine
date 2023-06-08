#include "CollisionComponent.h"
#include "Renderer.h"
#include "Transform.h"
#include "Scene.h"

dae::CollisionComponent::CollisionComponent(std::weak_ptr<GameObject> pOwner):
	Component(pOwner)
{
	
	m_pOwnerTransform = pOwner.lock()->GetComponent<TransformComponent>();
	m_pCurrentScene = pOwner.lock()->GetScene();
}

bool dae::CollisionComponent::IsBoxOverlapping(float xPos, float yPos, Box otherBox)
{
	auto ownerPos = m_pOwnerTransform.lock().get()->GetWorldPosition();

	if (ownerPos.x > xPos + otherBox.width || xPos > ownerPos.x + m_collisionBox.width) return false;
	
	if (ownerPos.y > yPos + otherBox.height || yPos > ownerPos.y + m_collisionBox.height) return false;

	return true;
}

bool dae::CollisionComponent::IsBoxOverlapping(GameObject* otherGameObject)
{
	auto transform = otherGameObject->GetComponent<TransformComponent>();

	if (!transform)
		return false;
	auto worldPosition = transform->GetWorldPosition();

	return IsBoxOverlapping(worldPosition.x, worldPosition.y, otherGameObject->GetComponent<CollisionComponent>()->GetMeasurements());
}


void dae::CollisionComponent::Update()
{
	auto objectsCollidingWith = m_pCurrentScene->HandleCollision(GetOwner().lock().get());

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
	auto ownerPos = m_pOwnerTransform.lock().get()->GetWorldPosition();
	Renderer::GetInstance().RenderCollisionRectangle(ownerPos.x, ownerPos.y, m_collisionBox.width, m_collisionBox.height, m_DebugRectColor.r, m_DebugRectColor.g, m_DebugRectColor.b);
#endif
}
