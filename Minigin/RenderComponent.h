#pragma once
#include "Component.h"
#include "Renderer.h"

namespace dae
{

	class RenderComponent final : public Component
	{
	public:
		RenderComponent();
		RenderComponent(std::weak_ptr<GameObject> pOwner);

		~RenderComponent();
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

		void Render() const override;


	private:
		const Renderer& m_Renderer;
		std::weak_ptr<GameObject> m_pOwner;
	};


}

