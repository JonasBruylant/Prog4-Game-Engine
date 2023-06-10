#pragma once
#include "GameObject.h"
#include <memory>


namespace dae 
{
	class Component
	{
	public:
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;


		virtual void Render() const {};
		virtual void RenderImGUI(){};
		virtual void Update() {}

	
	protected:
		explicit Component(GameObject* owner) : m_pComponentOwner{owner}{}
		GameObject* GetOwner() const { return m_pComponentOwner; }

	private:
		GameObject* m_pComponentOwner;
	};

}

