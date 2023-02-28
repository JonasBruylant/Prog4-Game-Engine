#pragma once
#include "GameObject.h"
#include <memory>


namespace dae 
{
	class Component
	{
	public:
		Component() = default;
		Component(std::weak_ptr<GameObject> owner ) {
			m_pComponentOwner = owner;
		}

		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Render() const {};
		virtual void Update(float deltaTime) { (void)deltaTime; }
	protected:

		std::weak_ptr<GameObject> m_pComponentOwner;
	};

}

