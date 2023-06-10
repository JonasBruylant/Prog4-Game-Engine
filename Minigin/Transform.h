#pragma once
#include <glm/glm.hpp>
#include "Component.h"


namespace dae
{

	class TransformComponent final : public Component
	{
	public:
		TransformComponent(GameObject* pOwner);

		~TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;

		const glm::vec3& GetLocalPosition() const;
		const glm::vec3& GetWorldPosition();

		void SetLocalPosition(const float x, const float y, const float z);
		void SetLocalPosition(const glm::vec3& position);
		void SetPositionDirty();

		void UpdateWorldPosition();
		void AddToLocalPosition(const glm::vec3& position);
		void AddToLocalPosition(const float x, const float y, const float z);

	private:
		glm::vec3 m_LocalPosition{0,0,0};
		glm::vec3 m_WorldPosition{0,0,0};
		bool m_positionIsDirty;

		GameObject* m_pOwner;
	};
}