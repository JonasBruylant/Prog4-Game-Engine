#pragma once
#include <memory>
#include <vector>

namespace dae
{

	class Component;

	class GameObject final
	{
	public:
		void Update(float deltaTime);
		void Render() const;

		template <typename T> T* AddComponent();
		template <typename T> T* GetComponent();
		template <typename T> void RemoveComponent();

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	protected:
		std::vector<std::weak_ptr<Component>> m_pComponentList;

	};
}
