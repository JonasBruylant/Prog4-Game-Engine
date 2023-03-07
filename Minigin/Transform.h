#pragma once
#include <glm/glm.hpp>
#include "Component.h"


namespace dae
{

	class TransformComponent final : public Component
	{
	public:
		TransformComponent(std::weak_ptr<GameObject> pOwner);
		const glm::vec3& GetLocalPosition() const;
		const glm::vec3& GetWorldPosition();

		const void SetLocalPosition(const float& x, const float& y, const float& z);
		const void SetLocalPosition(const glm::vec3& position);
		const void SetPositionDirty();

		void UpdateWorldPosition();

	private:
		glm::vec3 m_LocalPosition{0,0,0};
		glm::vec3 m_WorldPosition{0,0,0};
		bool m_positionIsDirty;
	};
}