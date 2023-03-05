#pragma once
#include <memory>
#include <vector>
#include <type_traits>
#include <iostream>
#include <algorithm>

namespace dae
{
	class Component;

	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		void Update(float deltaTime);
		void Render() const;

		template <typename T> std::shared_ptr<T> AddComponent();
		template <typename T> std::shared_ptr<T> GetComponent();
		template <typename T> void RemoveComponent();

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::vector<std::shared_ptr<Component>> m_pComponentList;

	};


	template<typename T>
	inline std::shared_ptr<T> GameObject::AddComponent()
	{
		auto gottenComponent = GetComponent<T>();

		if (gottenComponent == nullptr)
		{
			auto newComponent = std::make_shared<T>(shared_from_this());

			m_pComponentList.push_back(newComponent);

			return newComponent;
		}

		return gottenComponent;
	

	}

	template<typename T>
	inline std::shared_ptr<T> GameObject::GetComponent()
	{
		auto it = std::find_if(m_pComponentList.begin(), m_pComponentList.end(),
			[&](std::shared_ptr<Component> component)
			{
				return typeid(*component.get()).name() == typeid(T).name();
			});

		if (it == m_pComponentList.end())
			return nullptr;

		return std::static_pointer_cast<T>(*it);
	}

	template<typename T>
	inline void GameObject::RemoveComponent()
	{
		auto lambda = [&](std::shared_ptr<Component> component)
		{
			return typeid(*component.get()).name() == typeid(T).name();
		};
		m_pComponentList.erase(std::remove_if(m_pComponentList.begin(), m_pComponentList.end(),lambda), m_pComponentList.end());

	}

	
}
