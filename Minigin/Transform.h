#pragma once
#include <glm/glm.hpp>
#include "Component.h"


namespace dae
{

	class TransformComponent final : public Component
	{
	public:
		TransformComponent(std::weak_ptr<GameObject> pOwner);
		const glm::vec3& GetLocalPosition() const { return m_position; }
		const glm::vec3 GetWorldPosition() const;
		void SetPosition(float x, float y, float z);
	private:
		glm::vec3 m_position;
		std::weak_ptr<GameObject> m_pParent;
	};
}