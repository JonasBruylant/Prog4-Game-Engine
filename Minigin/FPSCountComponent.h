#pragma once
#include "Component.h"

namespace dae

{
	class FPSCountComponent final : public Component
	{
	public:
		FPSCountComponent(std::weak_ptr<GameObject> pOwner);

		~FPSCountComponent();

		void Update(float deltaTime) override;

		float GetFPS() { return m_FPS; }

		FPSCountComponent(const FPSCountComponent& other) = delete;
		FPSCountComponent(FPSCountComponent&& other) = delete;
		FPSCountComponent& operator=(const FPSCountComponent& other) = delete;
		FPSCountComponent& operator=(FPSCountComponent&& other) = delete;

	private:
		float m_FPS;
		float m_UpdateTime;
	};
}

