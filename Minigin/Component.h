#pragma once
//#include "MiniginPCH.h"
#include "GameObject.h"
#include <memory>


namespace dae 
{
	class Component
	{
	public:
		Component() = default;
		Component(std::shared_ptr<GameObject> owner ) {
			m_pComponentOwner = owner;
		}

		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		void Render() {};
		void Update(float deltaTime) {};
	protected:

		std::shared_ptr<GameObject> m_pComponentOwner;
	};

}

