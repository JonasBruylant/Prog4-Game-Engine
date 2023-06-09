#include "Scene.h"
#include "GameObject.h"
#include "CollisionComponent.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

std::vector<GameObject*> Scene::HandleCollision(GameObject* pGameObject)
{
	std::vector<GameObject*> currentCollisions;

	auto collisionComponent = pGameObject->GetComponent<CollisionComponent>();

	if(!collisionComponent)
		return currentCollisions;

	for (auto& currentObject : m_objects)
	{
		CheckGameObjectCollision(currentCollisions, currentObject, collisionComponent);
	}

	return currentCollisions;
}


Scene::Scene(const std::string& name) : m_name(name) {}

void dae::Scene::CheckGameObjectCollision(std::vector<GameObject*>& currentCollisions, std::shared_ptr < GameObject> pGameObject, std::shared_ptr<CollisionComponent>& pCurrentCollisionComponent)
{
	auto goCollision = pGameObject->GetComponent<CollisionComponent>();
	if (!goCollision || (pCurrentCollisionComponent == goCollision)) //if it doesn't exist, or it's his own collison, continue
		return;

	if (!pCurrentCollisionComponent->IsBoxOverlapping(pGameObject.get()))
		return;

	if (goCollision->GetTag() == pCurrentCollisionComponent->GetTag())
		return;
		
	currentCollisions.emplace_back(pGameObject.get());

	auto& children = pGameObject->GetChildren();
	if (children.size() == 0)
		return;

	for (auto& child : children)
	{
		CheckGameObjectCollision(currentCollisions, child, pCurrentCollisionComponent);
	}
}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	object->SetScene(this);
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();

	}

	//If marked for deletion, remove is from the scene
	m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), [](std::shared_ptr<GameObject>& object) {return object->GetMarkedForDelete(); }), m_objects.end());
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

