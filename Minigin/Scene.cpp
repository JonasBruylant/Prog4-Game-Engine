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
		auto currentCollisionComp = currentObject->GetComponent<CollisionComponent>();
		if (!currentCollisionComp || (collisionComponent == currentCollisionComp)) //if it doesn't exist, or it's his own collison, continue
			continue;

		if (!collisionComponent->IsBoxOverlapping(currentObject.get()))
			continue;

		currentCollisions.emplace_back(currentObject.get());
	}

	return currentCollisions;
}


Scene::Scene(const std::string& name) : m_name(name) {}

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

