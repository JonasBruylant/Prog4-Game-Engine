#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class CollisionComponent;


	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		void Update();
		void Render() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void SetIsActive(bool isActive) { m_IsActive = isActive; }
		bool GetIsActive() { return m_IsActive; }
		std::vector<GameObject*> HandleCollision(GameObject* pGameObject);
		std::string GetName() { return m_name; }
	private: 
		explicit Scene(const std::string& name);

		void CheckGameObjectCollision(std::vector<GameObject*>& currentCollisions, std::shared_ptr <GameObject> pGameObject, std::shared_ptr<CollisionComponent>& pCurrentCollisionComponent);

		std::string m_name;
		std::vector < std::shared_ptr<GameObject>> m_objects{};
		static unsigned int m_idCounter; 

		bool m_IsActive{ false };
	};

}
