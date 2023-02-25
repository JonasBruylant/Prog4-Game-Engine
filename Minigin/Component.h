#pragma once
#include <memory>
#include "GameObject.h"


namespace dae 
{
	class Component
	{
	public:
		Component() {};
		virtual ~Component() {}

		virtual void Update(float deltaTime) = 0;

	private:

		std::shared_ptr<GameObject> m_ComponentOwner;
	};

}

